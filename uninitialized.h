#pragma once

#include <iterator_traits.h>
#include <addressof.h>

namespace std
{

template <typename ForwardIterator, typename Allocator>
void destroy_range(ForwardIterator first, ForwardIterator last, Allocator& alloc)
{
    typedef typename std::iterator_traits<ForwardIterator>::value_type value_type;
    while (first != last)
    {
        alloc.destroy(first);
        ++first;
    }
}

template<typename ForwardIterator, typename Allocator, typename ... Args>
void uninitialized_construct_with_allocator(ForwardIterator first, ForwardIterator last, Allocator& alloc, Args&&... args)
{
    ForwardIterator current = first;
    try
    {
        for (; current != last; ++current)
        {
            alloc.construct(std::addressof(*current), std::forward<Args>(args)...);
        }
    }
    catch (...)
    {
        destroy_range(first, current, alloc);
        throw;
    }
}

template<typename InputIterator, typename ForwardIterator, typename Allocator>
ForwardIterator uninitialized_copy_with_allocator(InputIterator first, InputIterator last, ForwardIterator dest, Allocator& alloc)
{
    typedef typename std::iterator_traits<InputIterator>::value_type input_value_type;
    typedef typename std::iterator_traits<ForwardIterator>::value_type value_type;
    ForwardIterator current = first;
    try
    {
        for (; current != last; ++current, ++first)
        {
            alloc.construct(std::addressof(*dest), std::forward<input_value_type>(*first));
        }

        return dest;
    }
    catch (...)
    {
        destroy_range(dest, current, alloc);
        throw;
    }
}

}
