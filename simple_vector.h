#pragma once

#include <move.h>
#include <allocator.h>
#include <initializer_list.h>
#include <reverse_iterator.h>
#include <uninitialized.h>
#include <exception.h>
#include <algorithm.h>
#include <numeric_limits.h>

namespace std
{

template <typename Type, typename Allocator = allocator<Type>>
class vector : private Allocator // EBCO friendly
{
public:
    typedef Type* iterator;
    typedef const Type* const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef size_t size_type;

    typedef Allocator allocator_type;

    // ctor
    vector()
        : vector(Allocator())
    {
    }

    explicit vector(const Allocator& alloc)
        : Allocator(alloc)
        , m_first()
        , m_last()
        , m_endOfCapacity()
    {
    }

    vector(size_type count, const Type& value, const Allocator& alloc = Allocator())
        : Allocator(alloc)
    {
        resize(count, value);
    }

    explicit vector(size_type count, const Allocator& alloc = Allocator())
        : Allocator(alloc)
    {
        resize(count);
    }

    template <typename InputIterator>
    vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
        : Allocator(alloc)
    {
        insert
    }

    vector(const vector& other)
        : vector(other.cbegin(), other.cend(), other.get_allocator()) // actually should use std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())
    {
    }

    vector(const vector& other, const Allocator& alloc)
        : vector(other.cbegin(), other.cend(), alloc)
    {
    }

    vector(vector&& other)
        : Allocator(std::move(static_cast<Allocator&&>(other)))
        , m_first(std::move(other.m_first))
        , m_last(std::move(other.m_last))
        , m_endOfCapacity(std::move(other.m_endOfCapacity))
    {
        tidy(other);
    }

    vector(vector&& other, const Allocator& alloc)
        : Allocator(alloc)
        , m_first(std::move(other.m_first))
        , m_last(std::move(other.m_last))
        , m_endOfCapacity(std::move(other.m_endOfCapacity))
    {
        tidy(other);
    }

    vector(std::initializer_list<Type> init, const Allocator& alloc = Allocator())
        : vector(init.begin(), init.end(), alloc)
    {
    }

    ~vector()
    {
        erase(m_first, m_last);
        throw "not implemented";
    }


    //get_allocator method
    allocator_type get_allocator() const
    {
        return *this;
    }

    //*begin methods
    
    iterator begin()
    {
        return m_first;
    }

    const_iterator begin() const
    {
        return m_first;
    }

    const_iterator cbegin() const
    {
        return m_first;
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(m_last - 1);
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(m_last - 1);
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(m_last - 1);
    }



    //*end methods

    iterator end()
    {
        return m_last;
    }

    const_iterator end() const
    {
        return m_last;
    }

    const_iterator cend() const
    {
        return m_last;
    }

    reverse_iterator rend()
    {
        return reverse_iterator(m_first - 1);
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(m_first - 1);
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(m_first - 1);
    }


    //size method
    size_type size() const
    {
        return m_last - m_first;
    }

    size_type capacity() const
    {
        return m_endOfCapacity - m_first;
    }

    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max();
    }

    //empty method
    bool empty() const
    {
        return m_first == m_last;
    }


    // resize methods
    void resize(size_type count)
    {
        if (count > size())
        {
            reserve_for_future(count);
            uninitialized_construct_with_allocator(m_last, m_last + count, *this);
            m_last += count;
        }
        else if (count < size())
        {
            erase(begin() + count, end());
        }
    }

    void resize(size_type count, const Type& value)
    {
        if (count > size())
        {
            reserve_for_future(count);
            uninitialized_construct_with_allocator(m_last, m_last + count, *this, value);
            m_last += count;
        }
        else if (count < size())
        {
            erase(begin() + count, end());
        }
    }


    // reserve method
    void reserve(size_type newCapacity)
    {
        if (newCapacity > max_size())
        {
            throw std::length_error("");
        }

        if (newCapacity > capacity())
        {
            throw "not implemented";
        }
    }

    //erase methods
    iterator erase(const_iterator pos)
    {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        throw "not implemented";
        iterator newLast = rotate(cast_from_const(first), cast_from_const(last), m_last);

        destroy_range(newLast, m_last, *this);
        m_last = newLast;

        //return m_first + ()
    }

private:
    iterator m_first;
    iterator m_last;
    iterator m_endOfCapacity;


    static void tidy(vector& vec)
    {
        vec.m_first = 0;
        vec.m_last = 0;
        vec.m_endOfCapacity = 0;
    }

    void reserve_for_future(size_type desiredCapacity)
    {
        size_type exactCapacity = static_cast<size_type>(desiredCapacity * 1.5);
        exactCapacity = (exactCapacity > desiredCapacity) ? exactCapacity : (desiredCapacity + 1);
        reserve(exactCapacity);
    }

    static iterator cast_from_const(const_iterator pos)
    {
        return const_cast<iterator>(pos);
    }
};

}