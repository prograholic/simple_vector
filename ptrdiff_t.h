#pragma once

namespace std
{

typedef int ptrdiff_t; // FIXME: x86 MSVC specific
static_assert(sizeof(ptrdiff_t) == sizeof(size_t), "incorrect ptrdiff_t declaration");

}