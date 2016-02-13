#pragma once

//#include

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
struct get_pointer_type
{
    template <typename Alloc2>
    static auto deduce_type(Alloc2*) -> identity<typename Alloc2::pointer_type>;

    template <typename Alloc2>
    static auto deduce_type(...) -> identity<typename Allocator::value_type*>;
    
    typedef decltype(deduce_type(static_cast<Allocator*>(0))) deduced_identity_type;

    typedef typename deduced_identity_type::type type;
};

} // namespace detail

template <typename Allocator>
struct allocator_traits
{
    typedef Allocator allocator_type;
    typedef typename Allocator::value_type2 value_type;

    typedef typename detail::get_pointer_type<Allocator>::type pointer_type;



    static pointer_type allocate(Allocator& a, size_t n, const void* hint = 0)
    {
        return a.allocate2(n, hint);
    }

    static void deallocate(Allocator& a, pointer_type p, size_t n)
    {
        a.deallocate2(p, n);
    }
};

} // namespace std
