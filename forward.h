#pragma once

#include <type_traits.h>

namespace std
{

template<typename Type>
Type&& forward(typename remove_reference<Type>::type& arg) noexcept
{
    return static_cast<Type&&>(arg);
}

template<typename Type>
Type&& forward(typename remove_reference<Type>::type&& arg) noexcept
{
    static_assert(!is_lvalue_reference<Type>::value, "bad forward call");
    return static_cast<Type&&>(arg);
}

}