#pragma once

namespace std
{

struct nothrow_t
{
};

extern nothrow_t const nothrow;

}


void* operator new(size_t size);

void* operator new(size_t size, std::nothrow_t const& ) throw();

void* operator new [](size_t size);

void* operator new [](size_t size, std::nothrow_t const& ) throw();

void operator delete(void* block) throw();

void operator delete(void* block, std::nothrow_t const& ) throw();

void operator delete [](void* block) throw();

void operator delete [](void* block, std::nothrow_t const& ) throw();

void operator delete(void* block, size_t size) throw();

void operator delete [](void* block, size_t size) throw();

inline void* operator new(size_t /* size */, void* where) throw()
{
    return where;
}

inline void operator delete(void*, void*) throw()
{
    return;
}

inline void* operator new[] (size_t /* size */, void* where) throw()
{
    return where;
}

inline void operator delete[] (void*, void*) throw()
{
}
