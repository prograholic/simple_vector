#pragma once

#include <iterator_traits.h>

namespace std
{

template <typename Iterator>
class move_iterator
{
public:
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef Iterator pointer;
    typedef value_type&& reference;
    typedef Iterator iterator_type;

    move_iterator()
        : m_current()
    {
    }

    explicit move_iterator(iterator_type other)
        : m_current(other)
    {
    }

    template<class OtherIterator>
    move_iterator(const move_iterator<OtherIterator>& other)
        : m_current(other.base())
    {
    }

    template<class OtherIterator>
    move_iterator& operator=(const move_iterator<OtherIterator>& other)
    {	// assign with compatible base
        m_current = other.base();
        return *this;
    }

    Iterator base() const
    {
        return m_current;
    }

    reference operator*() const
    {
        return std::move(*m_current);
    }

    pointer operator->() const
    {
        return m_current;
    }

    move_iterator& operator++()
    {
        ++m_current;
        return *this;
    }

    move_iterator operator++(int)
    {
        move_iterator tmp = *this;
        ++m_current;
        return tmp;
    }

    move_iterator& operator--()
    {
        --m_current;
        return *this;
    }

    move_iterator operator--(int)
    {
        move_iterator tmp = *this;
        --m_current;
        return tmp;
    }

    move_iterator& operator+=(difference_type offset)
    {
        m_current += offset;
        return *this;
    }

    move_iterator operator+(difference_type offset) const
    {
        return move_iterator(m_current + offset);
    }

    move_iterator& operator-=(difference_type offset)
    {
        current -= offset;
        return *this;
    }

    move_iterator operator-(difference_type offset) const
    {
        return move_iterator(m_current - offset);
    }

    reference operator[](difference_type offset) const
    {
        return std::move(m_current[offset]);
    }

    difference_type operator-(const move_iterator& other) const
    {
        return (m_current - other.base());
    }

protected:
    iterator_type m_current;
};

template <typename Iterator1, typename Iterator2>
bool operator ==(const move_iterator<Iterator1>& left, const move_iterator<Iterator2>& right)
{
    return left.base() == right.base();
}

template <typename Iterator1, typename Iterator2>
bool operator !=(const move_iterator<Iterator1>& left, const move_iterator<Iterator2>& right)
{
    return left.base() != right.base();
}


template <typename Iterator>
move_iterator<Iterator> make_move_iterator(Iterator pos)
{
    return move_iterator<Iterator>(pos);
}

}
