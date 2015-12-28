#pragma once

#include <swap.h>

namespace std
{

template<class ForwardIterator1, class ForwardIterator2>
void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
   using std::swap;
   swap(*a, *b);
}

template <class ForwardIterator>
ForwardIterator rotate(ForwardIterator first, ForwardIterator n_first, ForwardIterator last)
{
    if (first == n_first)
    {
        return last;
    }

    if (n_first == last)
    {
        return first;
    }

    ForwardIterator next = n_first;
    while (first != next)
    {
        std::iter_swap(first++, next++);
        if (next == last)
        {
            next = n_first;
        }
        else if (first == n_first)
        {
            n_first = next;
        }
    }

    throw "not implemented";
    return first;
}

}
