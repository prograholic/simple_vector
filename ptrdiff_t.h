#pragma once

#include <type_traits.h>

namespace std
{

typedef decltype(declval<char*>() - declval<char*>()) ptrdiff_t;

static_assert(is_same<make_unsigned<ptrdiff_t>::type, size_t>::value, "incorrect ptrdiff_t declaration");

}