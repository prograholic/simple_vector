#pragma once

#include <type_traits.h>

namespace std
{

namespace detail
{

struct wrap_int
{
    wrap_int(int)
    {
    }
};

template <typename Type>
struct identity
{
    typedef Type type;
};

template <typename Allocator>
struct get_pointer
{
    template <typename Alloc2>
    static auto deduce_type(int) -> identity<typename Alloc2::pointer>;

    template <typename Alloc2>
    static auto deduce_type(wrap_int) -> identity<typename Allocator::value_type*>;
    
    typedef decltype(deduce_type<Allocator>(0)) deduced_identity_type;

    typedef typename deduced_identity_type::type type;
};

template <typename Allocator>
struct get_const_pointer
{
    template <typename Alloc2>
    static auto deduce_type(int)->identity<typename Alloc2::const_pointer>;

    template <typename Alloc2>
    static auto deduce_type(wrap_int)->identity<typename const Allocator::value_type*>;

    typedef decltype(deduce_type<Allocator>(0)) deduced_identity_type;

    typedef typename deduced_identity_type::type type;
};

template <typename Allocator>
struct get_size_type
{
    template <typename Alloc2>
    static auto deduce_type(int)->identity<typename Alloc2::size_type>;

    template <typename Alloc2>
    static auto deduce_type(wrap_int)->identity<size_t>;

    typedef decltype(deduce_type<Allocator>(0)) deduced_identity_type;

    typedef typename deduced_identity_type::type type;
};


struct call_construct
{
    template <typename Allocator, typename Type, typename... Args>
    static auto invoke(int, Allocator& alloc, Type* ptr, Args&&... args) -> typename void_t<decltype(alloc.construct(ptr, forward<Args>(args)...))>
    {
        alloc.construct(ptr, forward<Args>(args)...);
    }

    template <typename Allocator, typename Type, typename... Args>
    static void invoke(wrap_int, Allocator& /* alloc */, Type* ptr, Args&&... args)
    {
        ::new (static_cast<void*>(ptr)) Type(forward<Args>(args)...);
    }
};

struct call_destroy
{
    template <typename Allocator, typename Type>
    static auto invoke(int, Allocator& alloc, Type* ptr) -> typename void_t<decltype(alloc.destroy(ptr))>
    {
        alloc.destroy(ptr);
    }

    template <typename Allocator, typename Type>
    static void invoke(wrap_int, Allocator& /* alloc */, Type* ptr)
    {
        ptr->~Type();
    }
};



struct call_select_on_container_copy_construction
{
    template <typename Allocator>
    static auto invoke(int, const Allocator& alloc) -> typename void_t<decltype(alloc.select_on_container_copy_construction())>
    {
        return alloc.select_on_container_copy_construction();
    }

    template <typename Allocator>
    static auto invoke(wrap_int, const Allocator& alloc) -> Allocator
    {
        return alloc;
    }
};


} // namespace detail

template <typename Allocator>
struct allocator_traits
{
    typedef Allocator allocator_type;
    typedef typename Allocator::value_type value_type;

    typedef typename detail::get_pointer<Allocator>::type pointer;
    typedef typename detail::get_const_pointer<Allocator>::type const_pointer;

    typedef typename detail::get_size_type<Allocator>::type size_type;



    static pointer allocate(Allocator& a, size_type n, const void* hint = 0)
    {
        return a.allocate(n, hint);
    }

    static void deallocate(Allocator& a, pointer p, size_type n)
    {
        a.deallocate(p, n);
    }

    template <typename Type, typename... Args>
    static void construct(Allocator& alloc, Type* ptr, Args&&... args)
    {
        detail::call_construct::invoke(0, alloc, ptr, forward<Args>(args)...);
    }

    template <typename Type>
    static void destroy(Allocator& alloc, Type* ptr)
    {
        detail::call_destroy::invoke(0, alloc, ptr);
    }

    static Allocator select_on_container_copy_construction(const Allocator& alloc)
    {
        return detail::call_select_on_container_copy_construction::invoke(0, alloc);
    }
};

} // namespace std
