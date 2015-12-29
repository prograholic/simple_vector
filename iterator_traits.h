#pragma once

#include <ptrdiff_t.h>
#include <iterator_category.h>
#include <type_traits.h>

namespace std
{

template <typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
};

template <typename Type>
struct iterator_traits<Type*>
{
    typedef ptrdiff_t difference_type;
    typedef Type value_type;
    typedef Type* pointer;
    typedef Type& reference;
    typedef random_access_iterator_tag iterator_category;
};

template <typename Type>
struct iterator_traits<const Type*>
{
    typedef ptrdiff_t difference_type;
    typedef Type value_type;
    typedef const Type* pointer;
    typedef const Type& reference;
    typedef random_access_iterator_tag iterator_category;
};


template<typename, typename = void>
struct is_iterator : false_type
{
};


template <typename Type>
struct is_iterator<Type, typename param_tester<
    typename Type::iterator_category,
    typename Type::value_type,
    typename Type::difference_type,
    typename Type::pointer,
    typename Type::reference>::type> : true_type
{
};

template <typename Type>
struct is_iterator<Type *> : true_type
{
};

}
