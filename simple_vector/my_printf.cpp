#include <stdio.h>

extern "C"
{

int __cdecl my_printf(char const* const format, ...)
{
    int result;
    va_list argList;
    __crt_va_start(argList, format);
    result = _vfprintf_l(stdout, format, NULL, argList);
    __crt_va_end(argList);
    return result;
}

}

#include <allocator.h>
#include <iterator_traits.h>

namespace std2 {

    template <typename Type, typename Allocator = std::allocator<Type>>
    class vector;

    template <typename Type>
    struct allocator {
        typedef Type value_type;

        value_type* allocate(size_t n, const void* /* hint */ = 0) {
            return static_cast<value_type*>(::operator new(n * sizeof(value_type)));
        }

        void deallocate(value_type* p, size_t n) {
            ::operator delete(p, n * sizeof(value_type));
        }
    };


    typedef int* iterator;

    using namespace std;

    typedef decltype(declval<iterator>() - declval<iterator>()) difference_type;

    

    typedef iterator_traits<iterator>::difference_type difference_type;
    typedef make_unsigned<difference_type>             size_type;

    static_assert(is_same<difference_type, int>::value, "diff");


    template <typename Arg>
    int Sum(Arg arg)
    {
        return arg;
    }

    template <typename Arg, typename... Args>
    int Sum(Arg arg, Args&&... args)
    {
        return arg + Sum(forward<Args>(args)...);
    }


    template <typename... Args>
    int Foo(Args&&... args, ...)
    {
        return Sum(forward<Args>(args)...);
    }


    int Bar()
    {
        return Foo(1, 2, 3);
    }

#if 0
    template <typename Type, typename Allocator>
    struct vector_base: private Allocator // EBCO friendly
    {
        typedef allocator_traits<Allocator> vector_allocator_traits;

        iterator m_first;
        iterator m_last;
        iterator m_endOfCapacity;

        vector_base() : vector_base(Allocator()) {
        }

        vector_base(const Allocator& alloc) : Allocator(alloc) {
            tidy(*this);
        }

        vector_base(vector_base&& other) : Allocator(std::move(static_cast<Allocator&&>(other))) {
            swap_base(*this, static_cast<vector_base&>(other));
            tidy(other);
        }

        vector_base(vector_base&& other, const Allocator& alloc) : Allocator(alloc) {
            swap_base(*this, static_cast<vector_base&>(other));
            tidy(other);
        }


        vector_base& operator=(vector_base&& other);
        vector_base& operator=(const vector_base& other);
        ~vector_base();

        static pointer alloc(Allocator& alloc, size_type n);

        static void dealloc(Allocator& alloc, pointer p, size_type n);

        static void tidy(vector_base& vec);

        static void destroy_and_free(iterator first, iterator last, iterator endOfCapacity, Allocator& alloc);

        static iterator cast_from_const(const_iterator pos);

        static allocator_type& allocator_from(vector_base& vec);

        static const allocator_type& allocator_from(const vector_base& vec);



        allocator_type get_allocator() const;
        const_reference at(size_type pos) const;
        const_reference operator[](size_type pos) const;
        const_reference front() const;
        const_reference back() const;
        const value_type* data() const;
        const_iterator begin() const;
        const_iterator cbegin() const;
        const_reverse_iterator rbegin() const;
        const_reverse_iterator crbegin() const;
        const_iterator end() const;
        const_iterator cend() const;
        const_reverse_iterator rend() const;
        const_reverse_iterator crend() const;
        size_type size() const;
        size_type capacity() const;
        size_type max_size() const;
        bool empty() const;



        reference at(size_type pos);
        //[] operators
        reference operator[](size_type pos);

        

        //front methods
        reference front();

        

        //back methods
        reference back();

        

        //data methods
        value_type* data();

        



        //*begin methods

        iterator begin();

        reverse_iterator rbegin();

        //*end methods

        iterator end();
        reverse_iterator rend();



        void reserve(size_type newCapacity) {
            if (newCapacity > capacity()) {
                iterator newFirst = alloc(allocator_from(*this), newCapacity);

                try {
                    iterator newLast = detail::uninitialized_copy_a(make_move_iterator(m_first), make_move_iterator(m_last), newFirst, allocator_from(*this));

                    destroy_and_free(m_first, m_last, m_endOfCapacity, allocator_from(*this));

                    m_first = newFirst;
                    m_last = newLast;
                    m_endOfCapacity = m_first + newCapacity;
                } catch (...) {
                    dealloc(allocator_from(*this), newFirst, newCapacity);
                    throw;
                }
            }
        }



    private:

        void propagate_on_move_assignment(Allocator&& other, true_type);

        void propagate_on_move_assignment(Allocator&& /* other */, false_type);

        void propagate_on_move_assignment(Allocator&& other);

        void propagate_on_copy_assignment(const Allocator& other, true_type);

        void propagate_on_copy_assignment(const Allocator& /* other */, false_type);

        void propagate_on_copy_assignment(const Allocator& other);
    };




    template <typename FwdIt, typename Alloc>
    void destroy_range(FwdIt first, FwdIt last, Alloc& alloc)
    {
        while (first != last)
        {
            allocator_traits<Alloc>::destroy(alloc, first);
            ++first;
        }
    }

    template<typename FwdIt, typename Alloc, typename ... Args>
    void uninitialized_construct_with_allocator(FwdIt first, FwdIt last, Alloc& alloc, Args&&... args)
    {
        ForwardIterator current = first;
        try
        {
            for (; current != last; ++current)
            {
                allocator_traits<Alloc>::construct(alloc, addressof(*current), forward<Args>(args)...);
            }
        }
        catch (...)
        {
            destroy_range(first, current, alloc);
            throw;
        }
    }

    template<typename InputIterator, typename FwdIt, typename Alloc>
    FwdIt uninitialized_copy_with_allocator(InputIterator first, InputIterator last, FwdIt dest, Alloc& alloc)
    {
        FwdIt current = dest;
        try
        {
            for (; first != last; ++current, ++first)
            {
                allocator_traits<Alloc>::construct(alloc, addressof(*current), *first);
            }

            return current;
        }
        catch (...)
        {
            destroy_range(dest, current, alloc);
            throw;
        }
    }

    template<typename Type, typename Size, typename FwdIt, typename Alloc>
    FwdIt uninitialized_clone_n_with_allocator(const Type& value, Size n, FwdIt dest, Alloc& alloc)
    {
        ForwardIterator current = dest;
        try
        {
            for (; n > 0; ++current, --n)
            {
                allocator_traits<Alloc>::construct(alloc, addressof(*current), value);
            }

            return current;
        }
        catch (...)
        {
            destroy_range(dest, current, alloc);
            throw;
        }
    }
#endif //0
}