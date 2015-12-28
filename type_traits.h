#pragma once


namespace std
{

template<typename Type, Type Value>
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

template<bool Condition, typename Type = void>
struct enable_if
{
};

template<typename Type>
struct enable_if<true, Type>
{
    typedef Type type;
};

template<typename Type>
struct remove_reference
{
    typedef Type type;
};

template<typename Type>
struct remove_reference<Type&>
{
    typedef Type type;
};

template<typename Type>
struct remove_reference<Type&&>
{
    typedef Type type;
};



}