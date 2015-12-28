#pragma once

#include <forward.h>

namespace std
{

template <typename Type>
struct allocator
{
    template<typename OtherType, typename... Args>
    void construct(OtherType* p, Args&&... args)
    {
        ::new(static_cast<void *>(p)) OtherType(std::forward<Args>(args)...);
    }

};

}