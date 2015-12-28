#pragma once


namespace std
{

template <typename Type>
struct numeric_limits;


template <>
struct numeric_limits<size_t>
{
    static size_t max() noexcept
    {
        return static_cast<size_t>(~0);
    }
};

}
