#pragma once

namespace std
{

class exception
{
public:
    exception()
    {
    }

    exception(const exception& other)
    {
    }

    virtual ~exception()
    {
    }

    virtual const char* what() const
    {
        return "";
    }
};

class logic_error : public exception
{
public:
    template <typename Type>
    logic_error(const Type& /* value */) // FIXME: quick-n-dirty hack to overcome absence of std::string
        : exception()
    {
    }

    virtual const char* what() const
    {
        return "logic error";
    }
};

class length_error : public logic_error
{
public:
    template <typename Type>
    length_error(const Type& value)
        : logic_error(value)
    {
    }

    virtual const char* what() const
    {
        return "length error";
    }
};

class out_of_range: public logic_error
{
public:
    template <typename Type>
    out_of_range(const Type& value)
        : logic_error(value)
    {
    }

    virtual const char* what() const
    {
        return "out of range";
    }
};

}