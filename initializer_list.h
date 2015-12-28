#pragma once

namespace std
{
template<typename Type>
class initializer_list
{
public:
	//typedef Type value_type;
	//typedef const _Elem& reference;
	//typedef const _Elem& const_reference;
	//typedef size_t size_type;

	//typedef const _Elem* iterator;
	//typedef const _Elem* const_iterator;

	constexpr initializer_list() noexcept
        : m_first(0)
        , m_last(0)
    {
    }

    constexpr initializer_list(const Type* first, const Type* last) noexcept
		: m_first(first)
        , m_last(last)
    {
    }

    constexpr const Type* begin() const noexcept
    {
        return m_first;
    }

    constexpr const Type* end() const noexcept
    {
        return m_last;
    }

    constexpr size_t size() const noexcept
    {
        return m_last - m_first;
    }

private:
	const Type* m_first;
	const Type* m_last;
};

}