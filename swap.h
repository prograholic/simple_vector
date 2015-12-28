#pragma once

#include <move.h>

namespace std
{

template <typename Type>
void swap(Type& left, Type& right) // TODO: add noexcept spec
{
    Type tmp = std::move(left);
    left = std::move(right);
    right = std::move(tmp);
}

}