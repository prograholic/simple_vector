#pragma once

namespace std
{

template <typename ...>
struct param_tester
{
    typedef void type;
};


template <typename ... Args>
using void_t = typename param_tester<Args...>::type;

template <typename Type, Type Value>
struct integral_constant
{
    static const Type value = Value;

    typedef Type value_type;
    typedef integral_constant<Type, Value> type;

    operator value_type() const noexcept
    {
        return value;
    }
    
    value_type operator()() const noexcept
    {
        return value;
    }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <bool Condition, typename Type = void>
struct enable_if
{
};

template <typename Type>
struct enable_if<true, Type>
{
    typedef Type type;
};

template <typename Type>
struct remove_reference
{
    typedef Type type;
};

template <typename Type>
struct remove_reference<Type&>
{
    typedef Type type;
};

template <typename Type>
struct remove_reference<Type&&>
{
    typedef Type type;
};


template <typename Type>
struct is_function : public false_type
{
};

template <typename ReturnType, typename... ArgumentTypes>
struct is_function<ReturnType (ArgumentTypes...)> : public true_type // TODO: add calling convention
{
};

template <typename ReturnType, typename... ArgumentTypes>
struct is_function<ReturnType(ArgumentTypes..., ...)>: public true_type
{
};

template <typename Type>
struct is_lvalue_reference: std::false_type
{
};

template <typename Type>
struct is_lvalue_reference<Type&>: std::true_type
{
};


template <typename Base, typename Derived>
struct is_base_of : public integral_constant<bool, __is_base_of(Base, Derived)>
{
};

template<class OneType, class OtherType>
struct is_same: false_type
{
};

template<class Type>
struct is_same<Type, Type>: true_type
{
};

template<typename Type>
struct add_rvalue_reference
{
    typedef Type&& type;
};

template<>
struct add_rvalue_reference<void>
{
    typedef void type;
};

template<>
struct add_rvalue_reference<const void>
{
    typedef const void type;
};

template<>
struct add_rvalue_reference<volatile void>
{
    typedef volatile void type;
};

template<>
struct add_rvalue_reference<const volatile void>
{
    typedef const volatile void type;
};

template<class Type>
typename add_rvalue_reference<Type>::type declval();



namespace detail
{
template <typename Integer>
struct integer_traits;

template <>
struct integer_traits<char>
{
    typedef char type;

    static const bool is_integral = true;

    typedef signed char   signed_type;
    typedef unsigned char unsigned_type;
};

template <>
struct integer_traits<char16_t>
{
    typedef char16_t type;

    static const bool is_integral = true;
};

template <>
struct integer_traits<char32_t>
{
    typedef char32_t type;

    static const bool is_integral = true;
};

template <>
struct integer_traits<wchar_t>
{
    typedef wchar_t type;

    static const bool is_integral = true;
};

template <>
struct integer_traits<signed char>
{
    typedef signed char type;

    static const bool is_integral = true;

    typedef signed char   signed_type;
    typedef unsigned char unsigned_type;
};

template <>
struct integer_traits<unsigned char>
{
    typedef unsigned char type;

    static const bool is_integral = true;

    typedef signed char   signed_type;
    typedef unsigned char unsigned_type;
};

template <>
struct integer_traits<signed short>
{
    typedef signed short type;

    static const bool is_integral = true;

    typedef signed short   signed_type;
    typedef unsigned short unsigned_type;
};

template <>
struct integer_traits<unsigned short>
{
    typedef unsigned short type;

    static const bool is_integral = true;

    typedef signed short   signed_type;
    typedef unsigned short unsigned_type;
};

template <>
struct integer_traits<signed int>
{
    typedef signed int type;

    static const bool is_integral = true;

    typedef signed int   signed_type;
    typedef unsigned int unsigned_type;
};

template <>
struct integer_traits<unsigned int>
{
    typedef unsigned int type;

    static const bool is_integral = true;

    typedef signed int   signed_type;
    typedef unsigned int unsigned_type;
};

template <>
struct integer_traits<signed long>
{
    typedef signed long type;

    static const bool is_integral = true;

    typedef signed long   signed_type;
    typedef unsigned long unsigned_type;
};

template <>
struct integer_traits<unsigned long>
{
    typedef unsigned long type;

    static const bool is_integral = true;

    typedef signed long   signed_type;
    typedef unsigned long unsigned_type;
};

template <>
struct integer_traits<signed long long>
{
    typedef signed long long type;

    static const bool is_integral = true;

    typedef signed long long   signed_type;
    typedef unsigned long long unsigned_type;
};

template <>
struct integer_traits<unsigned long long>
{
    typedef unsigned long long type;

    static const bool is_integral = true;

    typedef signed long long   signed_type;
    typedef unsigned long long unsigned_type;
};




} // namespace detail

template <typename Integer>
struct make_unsigned
{
    typedef typename detail::integer_traits<Integer>::unsigned_type type;
};

template <typename Integer>
struct make_signed
{
    typedef typename detail::integer_traits<Integer>::signed_type type;
};

}

