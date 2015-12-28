#pragma once

#include <type_traits.h>

namespace std
{

template<typename Type>
typename enable_if<is_function<Type>::value, Type*>::type addressof(Type& arg)
{
    return arg;
}

template<typename Type>
typename enable_if<!is_function<Type>::value, Type*>::type addressof(Type& arg)
{
    return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
}

}
