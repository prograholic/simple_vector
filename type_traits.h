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

}

