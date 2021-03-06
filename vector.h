#pragma once

#include <move.h>
#include <allocator.h>
#include <allocator_traits.h>
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

template <typename Type, typename Allocator>
struct vector_base : private Allocator // EBCO friendly
{
    typedef Allocator allocator_type;

    typedef allocator_traits<allocator_type> vector_allocator_traits;


    typedef Type* iterator;
    typedef const Type* const_iterator;

    typedef Type value_type;
    typedef typename vector_allocator_traits::pointer pointer;
    typedef typename vector_allocator_traits::const_pointer const_pointer;

    typedef value_type& reference;
    typedef const value_type& const_reference;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef typename vector_allocator_traits::size_type size_type;
    typedef ptrdiff_t difference_type;


    iterator m_first;
    iterator m_last;
    iterator m_endOfCapacity;

    vector_base()
        : vector_base(Allocator())
    {
    }

    vector_base(const Allocator& alloc)
        : Allocator(alloc)
    {
        tidy(*this);
    }

    vector_base(vector_base&& other)
        : Allocator(std::move(static_cast<Allocator&&>(other)))
    {
        swap_base(*this, static_cast<vector_base&>(other));
        tidy(other);
    }

    vector_base(vector_base&& other, const Allocator& alloc)
        : Allocator(alloc)
    {
        swap_base(*this, static_cast<vector_base&>(other));
        tidy(other);
    }


    vector_base& operator=(vector_base&& other)
    {
        destroy_and_free(m_first, m_last, m_endOfCapacity, allocator_from(*this));

        propagate_on_move_assignment(static_cast<Allocator&&>(other));

        swap_base(*this, static_cast<vector_base&>(other));
        tidy(other);

        return *this;
    }

    vector_base& operator=(const vector_base& other)
    {
        propagate_on_copy_assignment(static_cast<const Allocator&>(other));

        return *this;
    }

    ~vector_base()
    {
        destroy_and_free(m_first, m_last, m_endOfCapacity, allocator_from(*this));
    }


    static pointer alloc(Allocator& alloc, size_type n)
    {
        return vector_allocator_traits::allocate(alloc, n);
    }

    static void dealloc(Allocator& alloc, pointer p, size_type n)
    {
        vector_allocator_traits::deallocate(alloc, p, n);
    }



    static void tidy(vector_base& vec)
    {
        vec.m_first = 0;
        vec.m_last = 0;
        vec.m_endOfCapacity = 0;
    }

    static void swap_base(vector_base& left, vector_base& right)
    {
        swap(left.m_first, right.m_first);
        swap(left.m_last, right.m_last);
        swap(left.m_endOfCapacity, right.m_endOfCapacity);
    }

    static void destroy_and_free(iterator first, iterator last, iterator endOfCapacity, Allocator& alloc)
    {
        detail::destroy_range(first, last, alloc);
        dealloc(alloc, first, endOfCapacity - first);
    }

    static iterator cast_from_const(const_iterator pos)
    {
        return const_cast<iterator>(pos);
    }

    static allocator_type& allocator_from(vector_base& vec)
    {
        return vec;
    }

    static const allocator_type& allocator_from(const vector_base& vec)
    {
        return vec;
    }

private:

    static void propagate_on_move_assignment(Allocator&& other, true_type)
    {
        static_cast<Allocator&>(*this) = std::move(other);
    }

    static void propagate_on_move_assignment(Allocator&& /* other */, false_type)
    {
        // does nothing
    }
    
    static void propagate_on_move_assignment(Allocator&& other)
    {
        typedef vector_allocator_traits::propagate_on_container_move_assignment tag;
        propagate_on_move_assignment(forward<Allocator>(other), tag());
    }

    static void propagate_on_copy_assignment(const Allocator& other, true_type)
    {
        static_cast<Allocator&>(*this) = alloc;
    }

    static void propagate_on_copy_assignment(const Allocator& /* other */, false_type)
    {
        // does nothing
    }

    static void propagate_on_copy_assignment(const Allocator& other)
    {
        typedef vector_allocator_traits::propagate_on_container_copy_assignment tag;
        propagate_on_copy_assignment(other, tag());
    }
};

template <typename Type, typename Allocator = allocator<Type>>
class vector : private vector_base<Type, Allocator> // EBCO friendly
{
public:

    typedef vector_base<Type, Allocator> base_type;

    using typename base_type::iterator;
    using typename base_type::const_iterator;

    using typename base_type::value_type;
    using typename base_type::pointer;
    using typename base_type::const_pointer;

    using typename base_type::reference;
    using typename base_type::const_reference;

    using typename base_type::reverse_iterator;
    using typename base_type::const_reverse_iterator;

    using typename base_type::size_type;
    using typename base_type::difference_type;

    // BUG? cannot use `using` for allocator_type
    typedef typename base_type::allocator_type allocator_type;

    // ctor
    vector()
        : vector_base()
    {
    }

    explicit vector(const Allocator& alloc)
        : vector_base(alloc)
    {
    }

    vector(size_type count, const Type& value, const Allocator& alloc = Allocator())
        : vector_base(alloc)
    {
        resize(count, value);
    }

    explicit vector(size_type count, const Allocator& alloc = Allocator())
        : vector_base(alloc)
    {
        resize(count);
    }

    template <typename InputIterator>
    vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
        : vector_base(alloc)
    {
        assign(first, last);
    }

    vector(const vector& other)
        : vector(other.cbegin(),
                 other.cend(),
                 allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator()))
    {
    }

    vector(const vector& other, const Allocator& alloc)
        : vector(other.cbegin(), other.cend(), alloc)
    {
    }

    vector(vector&& other)
        : vector_base(std::forward<vector_base>(other))
    {
    }

    vector(vector&& other, const Allocator& alloc)
        : vector_base(std::forward<vector_base>(other), alloc)
    {
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
            static_cast<vector_base&>(*this) = static_cast<const vector_base&>(other);

            assign(other.begin(), other.end());
        }

        return *this;
    }

    vector& operator=(vector&& other)
    {
        if (&other != this)
        {
            static_cast<vector_base&>(*this) = std::move(static_cast<vector_base&&>(other));
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
    }


    //get_allocator method
    allocator_type get_allocator() const
    {
        return allocator_from(*this);
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

    //front methods
    reference front()
    {
        return *m_first;
    }

    const_reference front() const
    {
        return *m_first;
    }

    //back methods
    reference back()
    {
        return *(m_last - 1);
    }

    const_reference back() const
    {
        return *(m_last - 1);
    }

    //data methods
    value_type* data()
    {
        return m_first;
    }

    const value_type* data() const
    {
        return m_first;
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
            detail::uninitialized_construct_a(m_last, newLast, allocator_from(*this));
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
            detail::uninitialized_construct_a(m_last, newLast, allocator_from(*this), value);
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
            iterator newFirst = alloc(allocator_from(*this), newCapacity);

            try
            {
                relocate(newFirst, newCapacity);
            }
            catch (...)
            {
                dealloc(allocator_from(*this), newFirst, newCapacity);
                throw;
            }
        }
    }

    //shrink_to_fit method
    void shrink_to_fit()
    {
        vector tmp(std::move(*this)); // this become empty

        *this = std::move(vector(std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()), tmp.get_allocator()));
    }


    //clear method
    void clear()
    {
        erase(m_first, m_last);
    }


    //erase methods
    iterator erase(const_iterator pos)
    {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        iterator newLast = rotate(cast_from_const(first), cast_from_const(last), m_last);

        detail::destroy_range(newLast, m_last, allocator_from(*this));
        m_last = newLast;

        return cast_from_const(first);
    }

    //emplace method
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        return insert_with_perfect_fwd(pos, std::forward<Args>(args)...);
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
        detail::uninitialized_fill_n_a(m_last, count, value, allocator_from(*this));
        m_last += count;
        rotate(m_first + offset, m_last - count, m_last);

        return m_first + offset;
    }

    template <typename InputIterator>
    typename enable_if<is_iterator<InputIterator>::value, iterator>::type
    insert(const_iterator pos, InputIterator first, InputIterator last)
    {
        return insert_range(pos, first, last);
    }

    iterator insert(const_iterator pos, std::initializer_list<Type> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    //push_back methods
    void push_back(const value_type& value)
    {
        insert(end(), value);
    }

    void push_back(value_type&& value)
    {
        insert(end(), std::forward<value_type>(value));
    }

    //emplace_back
    template <typename... Args>
    void emplace_back(Args&&... args)
    {
        insert(end(), std::forward<Args>(args)...);
    }

private:

    void grow(size_type desiredCapacity)
    {
        if (desiredCapacity > capacity())
        {
            const size_type computedCapacity = static_cast<size_type>(capacity() * 1.5);
            const size_type exactCapacity = max(desiredCapacity, computedCapacity);
            reserve(exactCapacity);
        }
    }

    void relocate(iterator newFirst, size_type newCapacity)
    {
        iterator newLast = detail::uninitialized_copy_a(make_move_iterator(m_first), make_move_iterator(m_last), newFirst, allocator_from(*this));

        destroy_and_free(m_first, m_last, m_endOfCapacity, allocator_from(*this));

        m_first = newFirst;
        m_last = newLast;
        m_endOfCapacity = m_first + newCapacity;
    }

    template <typename... Args>
    iterator insert_with_perfect_fwd(const_iterator pos, Args&&... args)
    {
        difference_type offset = pos - cbegin();

        grow(size() + 1);
        detail::uninitialized_construct_a(m_last, m_last + 1, allocator_from(*this), std::forward<Args>(args)...);
        m_last += 1;
        rotate(m_first + offset, m_last - 1, m_last);

        return m_first + offset;
    }

    template <typename InputIterator>
    typename std::enable_if<std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>::value, iterator>::type
    insert_range(const_iterator pos, InputIterator first, InputIterator last)
    {
        difference_type offset = pos - cbegin();
        difference_type count = std::distance(first, last);

        grow(size() + count);
        detail::uninitialized_copy_a(first, last, m_last, allocator_from(*this));

        m_last += count;
        rotate(m_first + offset, m_last - count, m_last);

        return m_first + offset;
    }
};

}