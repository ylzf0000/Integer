//Integer.h
#pragma once
#include <iostream>
#include <functional>
#include <cassert>
#include <type_traits>
//#define INTEGER_NAMESPACE XXX
#ifdef  INTEGER_NAMESPACE
#define INTEGER_NAMESPACE_BEGIN  namespace INTEGER_NAMESPACE{
#define INTEGER_NAMESPACE_END    }
#else
#define INTEGER_NAMESPACE_BEGIN
#define INTEGER_NAMESPACE_END
#endif // INTEGER_NAMESPACE

INTEGER_NAMESPACE_BEGIN
template<unsigned N, unsigned Cap>
class __Integer;

template<unsigned N, unsigned Cap>
bool __less(const __Integer<N, Cap> &left, const __Integer<N, Cap> &right);

template<unsigned N, unsigned Cap>
bool __equal(const __Integer<N, Cap> &left, const __Integer<N, Cap> &right);

template<unsigned N, unsigned Cap>
std::ostream & __print(std::ostream &os, const __Integer<N, Cap> &right);

template<unsigned N = 1000, unsigned Cap = 1000000000>
class __Integer
{
public:
    using self = __Integer<N, Cap>;
    using size_type = size_t;
    __Integer() = default;
    __Integer(const self &right) = default;
    self &operator=(const self &right) = default;
    template<typename T> __Integer(T right);
    template<typename T> auto operator=(T right)->self&;
    friend bool __less<N, Cap>(const __Integer<N, Cap> &left, const __Integer<N, Cap> &right);
    friend bool __equal<N, Cap>(const __Integer<N, Cap> &left, const __Integer<N, Cap> &right);
    friend std::ostream & __print<N, Cap>(std::ostream &os, const __Integer<N, Cap> &right);
    friend bool operator<(const self &left, const self &right) { return __less(left, right); }
    friend bool operator>(const self &left, const self &right) { return !(left < right) && !(left == right); }
    friend bool operator==(const self &left, const self &right) { return __equal(left, right); }
    friend bool operator!=(const self &left, const self &right) { return !(left == right); }
    friend bool operator<=(const self &left, const self &right) { return left < right || left == right; }
    friend bool operator>=(const self &left, const self &right) { return left > right || left == right; }
    self operator-()const { self right(*this); right.m_sign = !right.m_sign; right.__fix_if_zero(); return right; }
    auto operator+=(const self &right)->self&;
    self &operator-=(const self &right) { return this->operator+=(-__Integer(right)); }
    auto operator*=(const self &right)->self&;
    auto operator/=(const self &right)->self&;
    self &operator++() { return *this += 1; }
    self &operator--() { return *this -= 1; }
    self operator++(int) { self right(*this); *this += 1; return right; }
    self operator--(int) { self right(*this); *this -= 1; return right; }
    friend self operator+(const self &left, const self &right) { return __Integer(left) += right; }
    friend self operator-(const self &left, const self &right) { return __Integer(left) -= right; }
    friend self operator*(const self &left, const self &right) { return __Integer(left) *= right; }
    friend self operator/(const self &left, const self &right) { return __Integer(left) /= right; }
    friend std::ostream &operator<<(std::ostream &os, const self &right) { return __print(os, right); }
    friend self abs(const self &right) { self ret(right); ret.m_sign = false; return ret; }

private:
    static unsigned __f_bitnum(unsigned x) {
        unsigned ret = 0;
        while (x)
            ++ret, x /= 10;
        return ret;
    }

    struct SIGNED_T {};
    struct UNSIGNED_T {};

    template<bool b> struct SIGN_TRAIT {};
    template<> struct SIGN_TRAIT<true> { using type = SIGNED_T; /*static constexpr bool value = true;*/ };
    template<> struct SIGN_TRAIT<false> { using type = UNSIGNED_T;/* static constexpr bool value = false;*/ };
    template<typename T> void __assign(T right, SIGNED_T);
    template<typename T> void __assign(T right, UNSIGNED_T);
    /*两个都正或两个都负*/
    auto __add_all_positive_or_negative(const self &right)->self&;
    /*一正一负或一负一正*/
    auto __add_one_positive_another_negative(const self &right)->self&;
    /*减法：两个数均为正数，要求*this > right*/
    auto __sub_all_positive_and_this_greater_than_right(const self &right)->self&;
    /*当*this == 0时，*this视为正数: m_sign = false*/
    void __fix_if_zero() { m_sign = __equal_zero() ? false : m_sign; }
    bool __equal_zero() { return m_length == 0; }
    int m_data[N] = { 0 };
    size_type m_length = 0;
    bool m_sign = false;
};

using Integer = __Integer<>;
template class __Integer<>;

template<unsigned N, unsigned Cap>
template<typename T>
inline __Integer<N, Cap>::__Integer(T right)
{
    operator=(right);
}

template<unsigned N, unsigned Cap>
template<typename T>
inline auto __Integer<N, Cap>::operator=(T right) -> self &
{
    static_assert(std::is_integral<T>::value, "A integer was required.");
    __assign<T>(right, typename SIGN_TRAIT<std::is_signed<T>::value>::type());
    return *this;
}

template<unsigned N, unsigned Cap>
template<typename T>
inline void __Integer<N, Cap>::__assign(T right, SIGNED_T)
{
    static_assert(std::is_signed<T>::value, "A signed integer was required.");
    m_sign = right < 0;
    right = m_sign ? -right : right;
    m_length = 0;
    while (right) {
        m_data[m_length++] = right % Cap;
        right /= Cap;
    }
}

template<unsigned N, unsigned Cap>
template<typename T>
inline void __Integer<N, Cap>::__assign(T right, UNSIGNED_T)
{
    static_assert(std::is_unsigned<T>::value, "A unsigned integer was required.");
    m_sign = false;
    m_length = 0;
    while (right) {
        m_data[m_length++] = right % Cap;
        right /= Cap;
    }
}

template<unsigned N, unsigned Cap>
inline auto __Integer<N, Cap>::operator+=(const self & right) -> self &
{
    if (m_sign ^ right.m_sign)  //一正一负或一负一正
        return __add_one_positive_another_negative(right);
    else                        //两个都正或两个都负
        return __add_all_positive_or_negative(right);
}

template<unsigned N, unsigned Cap>
inline auto __Integer<N, Cap>::operator*=(const self & right) -> self &
{
    __Integer<N, Cap> c;
    c.m_sign = this->m_sign ^ right.m_sign;
    for (size_type i = 0; i < m_length; ++i)
        for (size_type j = 0; j < right.m_length; ++j) {
            long long tmp = (long long)m_data[i] * right.m_data[j];
            c.m_data[i + j] += static_cast<unsigned>(tmp % Cap);
            c.m_data[i + j + 1] += static_cast<unsigned>(tmp / Cap);
        }
    c.m_length = m_length + right.m_length - 1;
    if (c.m_data[c.m_length] != 0)
        ++c.m_length;
    c.__fix_if_zero();
    return *this = c;
}

template<unsigned N, unsigned Cap>
inline auto __Integer<N, Cap>::operator/=(const self & right) -> self &
{
    assert(right != 0);
    self a_abs = abs(*this);
    const self &b_abs = abs(right);
    if (a_abs < b_abs)
        return *this = 0;
    self ret = 0;
    while (a_abs >= b_abs) {
        self b = b_abs;
        self tmp = 1;
        while (b * 10 <= a_abs)
            b *= 10, tmp *= 10;
        int i;
        for (i = 2; i * b <= a_abs; ++i);
        --i;
        b *= i;
        tmp *= i;
        ret += tmp;
        a_abs -= b;
    }
    ret.m_sign = m_sign ^ right.m_sign;
    return *this = ret;
}

template<unsigned N, unsigned Cap>
inline auto __Integer<N, Cap>::__add_all_positive_or_negative(const self & right) -> self &
{
    size_type max_len = m_length > right.m_length ? m_length : right.m_length;
    for (size_type i = 0; i < max_len; ++i)
    {
        m_data[i] += right.m_data[i];
        m_data[i + 1] += m_data[i] / Cap;
        m_data[i] %= Cap;
    }
    if (m_data[max_len])
        ++max_len;
    m_length = max_len;
    return *this;
}

template<unsigned N, unsigned Cap>
inline auto __Integer<N, Cap>::__add_one_positive_another_negative(const self & right) -> self &
{
    self a_abs = abs(*this), b_abs = abs(right);
    if (m_sign && !right.m_sign) {   // a < 0, b > 0
        if (a_abs < b_abs)              // |a| < |b|
            return *this = b_abs.__sub_all_positive_and_this_greater_than_right(a_abs);
        else                            // |a| >= |b|
            return *this = -a_abs.__sub_all_positive_and_this_greater_than_right(b_abs);
    }
    else {                          // a > 0, b < 0
        if (a_abs < b_abs)              // |a| < |b|
            return *this = -b_abs.__sub_all_positive_and_this_greater_than_right(a_abs);
        else                            // |a| >= |b|
            return *this = a_abs.__sub_all_positive_and_this_greater_than_right(b_abs);
    }
}

template<unsigned N, unsigned Cap>
inline auto __Integer<N, Cap>::__sub_all_positive_and_this_greater_than_right(const self & right) -> self &
{
    for (size_type i = 0; i < m_length; ++i) {
        m_data[i] -= right.m_data[i];
        if (m_data[i] < 0) {
            m_data[i] += Cap;
            --m_data[i + 1];
        }
    }
    while (m_data[m_length - 1] == 0)
        --m_length;
    __fix_if_zero();
    return *this;
}

template<unsigned N, unsigned Cap>
bool __less(const __Integer<N, Cap> &left, const __Integer<N, Cap> &right)
{
    if (left.m_sign && !right.m_sign)
        return true;
    if (left.m_sign && right.m_sign)
        return false;
    bool ret = false;
    if (left.m_length != right.m_length) {
        ret = left.m_length < right.m_length;
        goto RET;
    }
    for (int i = left.m_length - 1; i >= 0; --i) {
        if (left.m_data[i] != right.m_data[i]) {
            ret = left.m_data[i] < right.m_data[i];
            goto RET;
        }
    }
RET:;
    ret = left.m_sign ? !ret : ret;
    return ret;
}

template<unsigned N, unsigned Cap>
bool __equal(const __Integer<N, Cap> &left, const __Integer<N, Cap> &right)
{
    if (left.m_sign != right.m_sign || left.m_length != right.m_length)
        return false;
    for (typename __Integer<N, Cap>::size_type i = 0; i < left.m_length; ++i)
        if (left.m_data[i] != right.m_data[i])
            return false;
    return true;
}

template<unsigned N, unsigned Cap>
std::ostream & __print(std::ostream & os, const __Integer<N, Cap>& right)
{
    if (right == 0)
        return os << 0;
    if (right.m_sign)
        os << '-';
    unsigned cap_bit_num = __Integer<N, Cap>::__f_bitnum(Cap) - 1;
    for (int i = right.m_length - 1, j = 0; i >= 0; --i, ++j) {
        if (j) {
            os.width(cap_bit_num);
            os.fill('0');
        }
        os << right.m_data[i];
        //unsigned bit_num = __Integer<N, Cap>::__f_bitnum(right.m_data[i]);
        //unsigned zero_num = cap_bit_num - bit_num;
        //if (j)
        //    for (unsigned k = 0; k < zero_num; ++k)
        //        os << '0';
        //if (right.m_data[i])
        //    os << right.m_data[i];
    }
    return os;
}

INTEGER_NAMESPACE_END