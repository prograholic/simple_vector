#pragma once

#include <forward.h>

#include <new.h>

namespace std
{

template <typename Type>
struct allocator // implements minimal allocator
{
    typedef Type value_type;

    value_type* allocate(size_t n, const void* /* hint */ = 0)
    {
        return static_cast<value_type*>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(value_type* p, size_t n)
    {
        ::operator delete(p, n * sizeof(value_type));
    }

};

}