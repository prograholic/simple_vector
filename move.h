#pragma once

#include <type_traits.h>

namespace std
{
template<typename Type>
constexpr typename remove_reference<Type>::type&& move(Type&& value) noexcept
{
    return static_cast<typename remove_reference<Type>::type&&>(value);
}

}