#pragma once


namespace std
{

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