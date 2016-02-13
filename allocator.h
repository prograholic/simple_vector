#pragma once

#include <forward.h>

#include <new.h>

namespace std
{

template <typename Type>
struct allocator
{
    typedef Type* pointer;
    typedef size_t size_type;
    typedef Type value_type;

    template <typename OtherType, typename... Args>
    void construct(OtherType* p, Args&&... args)
    {
        ::new(static_cast<void *>(p)) OtherType(std::forward<Args>(args)...);
    }

    template <typename OtherType>
    void destroy(OtherType* p)
    {
        (void) p;
        p->~OtherType();
    }

    pointer allocate(size_type n, const void* /* hint */ = 0)
    {
        return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(pointer p, size_type n)
    {
        ::operator delete(p, n * sizeof(value_type));
    }

};

}