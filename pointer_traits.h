#pragma once

#include <ptrdiff_t.h>

namespace std
{

template <typename Pointer>
struct pointer_traits;


template <typename Type>
struct pointer_traits<Type*>
{
    typedef Type* pointer;
    typedef Type element_type;
    typedef ptrdiff_t difference_type;

    template <typename Other>
    using rebind = Other*;
};

} // namespace std
