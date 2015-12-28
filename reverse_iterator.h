#pragma once


namespace std
{

template <typename Iterator>
struct reverse_iterator
{
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