#pragma once

#include <move.h>
#include <allocator.h>
#include <initializer_list.h>
#include <reverse_iterator.h>
#include <uninitialized.h>
#include <exception.h>
#include <algorithm.h>
#include <numeric_limits.h>
#include <move_iterator.h>
#include <type_traits.h>

namespace std
{

template <typename Type, typename Allocator = allocator<Type>>
class vector : private Allocator // EBCO friendly
{
public:
    typedef Type* iterator;
    typedef const Type* const_iterator;

    typedef Type value_type;

    typedef value_type& reference;
    typedef const value_type& const_reference;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

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
        assign(first, last);
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

    //assignment operators
    vector& operator=(const vector& other)
    {
        if (&other != this)
        {
            static_cast<Allocator&>(*this) = static_cast<const Allocator&>(other);
            assign(other.begin(), other.end());
        }

        return *this;
    }

    vector& operator=(vector&& other)
    {
        if (&other != this)
        {
            destroy_and_free(m_first, m_last, m_endOfCapacity, *this);

            static_cast<Allocator&>(*this) = std::move(static_cast<Allocator&&>(other));
            
            m_first = other.m_first;
            m_last = other.m_last;
            m_endOfCapacity = other.m_endOfCapacity;

            tidy(other);
        }

        return *this;
    }

    vector& operator=(std::initializer_list<Type> ilist)
    {
        assign(ilist.begin(), ilist.end());

        return *this;
    }


    // assign methods
    void assign(size_type count, const Type& value)
    {
        erase(m_first, m_last);
        resize(count, value);
    }

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        erase(m_first, m_last);
        insert(begin(), first, last);
    }

    void assign(std::initializer_list<Type> ilist)
    {
        assign(ilist.begin(), ilist.end());
    }


    //dtor
    ~vector()
    {
        destroy_and_free(m_first, m_last, m_endOfCapacity, allocator_from(*this));
    }


    //get_allocator method
    allocator_type get_allocator() const
    {
        return *this;
    }


    //at methods
    reference at(size_type pos)
    {
        if (pos >= size())
        {
            throw out_of_range("");
        }

        return m_first[pos];
    }

    const_reference at(size_type pos) const
    {
        if (pos >= size())
        {
            throw out_of_range("");
        }

        return m_first[pos];
    }

    //[] operators
    reference operator[](size_type pos)
    {
        return m_first[pos];
    }

    const_reference operator[](size_type pos) const
    {
        return m_first[pos];
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
            grow(count);
            iterator newLast = m_first + count;
            uninitialized_construct_with_allocator(m_last, newLast, allocator_from(*this));
            m_last = newLast;
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
            grow(count);
            iterator newLast = m_first + count;
            uninitialized_construct_with_allocator(m_last, newLast, allocator_from(*this), value);
            m_last = newLast;
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
            iterator newFirst = allocate(newCapacity);

            try
            {
                iterator newLast = uninitialized_copy_with_allocator(make_move_iterator(m_first), make_move_iterator(m_last), newFirst, allocator_from(*this));

                destroy_and_free(m_first, m_last, m_endOfCapacity, *this);

                m_first = newFirst;
                m_last = newLast;
                m_endOfCapacity = m_first + newCapacity;
            }
            catch (...)
            {
                deallocate(newFirst, newCapacity);
                throw;
            }
        }
    }

    //erase methods
    iterator erase(const_iterator pos)
    {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        iterator newLast = rotate(cast_from_const(first), cast_from_const(last), m_last);

        destroy_range(newLast, m_last, allocator_from(*this));
        m_last = newLast;

        return cast_from_const(first);
    }


    //insert methods
    iterator insert(const_iterator pos, const Type& value)
    {
        return insert_with_perfect_fwd(pos, value);
    }

    iterator insert(const_iterator pos, Type&& value)
    {
        return insert_with_perfect_fwd(pos, std::forward<Type>(value));
    }

    iterator insert(const_iterator pos, size_type count, const Type& value)
    {
        difference_type offset = pos - cbegin();

        grow(size() + count);
        uninitialized_copy_with_allocator_n(value, count, m_last, allocator_from(*this));
        m_last += count;
        rotate(m_first + offset, m_last - count, m_last);

        return m_first + offset;
    }

    template <typename InputIterator>
    typename enable_if<is_iterator<InputIterator>::value, iterator>::type
    insert(const_iterator pos, InputIterator first, InputIterator last)
    {
        return insert_iterator_range(pos, first, last);
    }

    iterator insert(const_iterator pos, std::initializer_list<Type> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
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

    static void destroy_and_free(iterator first, iterator last, iterator endOfCapacity, Allocator& alloc)
    {
        destroy_range(first, last, alloc);
        alloc.deallocate(first, endOfCapacity - first);
    }

    void grow(size_type desiredCapacity)
    {
        if (desiredCapacity > capacity())
        {
            const size_type computedCapacity = static_cast<size_type>(capacity() * 1.5);
            const size_type exactCapacity = max(desiredCapacity, computedCapacity);
            reserve(exactCapacity);
        }
    }

    static iterator cast_from_const(const_iterator pos)
    {
        return const_cast<iterator>(pos);
    }

    static allocator_type& allocator_from(vector& vec)
    {
        return vec;
    }

    template <typename... Args>
    iterator insert_with_perfect_fwd(const_iterator pos, Args&&... args)
    {
        difference_type offset = pos - cbegin();

        grow(size() + 1);
        uninitialized_construct_with_allocator(m_last, m_last + 1, allocator_from(*this), std::forward<Args>(args)...);
        m_last += 1;
        rotate(m_first + offset, m_last - 1, m_last);

        return m_first + offset;
    }

    template <typename InputIterator>
    typename std::enable_if<std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>::value, iterator>::type
    insert_iterator_range(const_iterator pos, InputIterator first, InputIterator last)
    {
        difference_type offset = pos - cbegin();
        difference_type count = std::distance(first, last);

        grow(size() + count);
        uninitialized_copy_with_allocator(first, last, m_last, allocator_from(*this));
        
        m_last += count;
        rotate(m_first + offset, m_last - count, m_last);

        return m_first + offset;
    }
};

}