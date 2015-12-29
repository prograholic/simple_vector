#pragma once


#include <iterator_traits.h>

namespace std
{

template <typename Iterator>
struct reverse_iterator
{
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::reference reference;

    reverse_iterator()
        : m_current()
    {
    }

    explicit reverse_iterator(Iterator it)
        : m_current(it)
    {
    }

    Iterator base() const
    {
        return m_current;
    }

private:
    Iterator m_current;
};

template <typename Iterator1, typename Iterator2>
bool operator ==(const reverse_iterator<Iterator1>& left, const reverse_iterator<Iterator2>& right)
{
    return left.base() == right.base();
}

template <typename Iterator1, typename Iterator2>
bool operator !=(const reverse_iterator<Iterator1>& left, const reverse_iterator<Iterator2>& right)
{
    return left.base() != right.base();
}

template<typename Iterator1, typename Iterator2>
auto inline operator-(const reverse_iterator<Iterator1>& left, const reverse_iterator<Iterator2>& right) -> decltype(right.base() - left.base())
{
    return right.base() - left.base();
}

}