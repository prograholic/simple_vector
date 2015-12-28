#include <simple_vector.h>

#include <stdio.h>

#define STR(x) #x
#define STR2(x) STR(x)
#define CHECK(val) do { if (!(val)) throw STR(val) " at " STR2(__FILE__) "(" STR2(__LINE__) ")";} while(0)

int ProcessException()
{
    try
    {
        throw;
    }
    catch (const char* msg)
    {
        printf("%s\n", msg);
        return 1;
    }
    catch (...)
    {
        printf("UNKNOWN ERROR!!!\n");
        return 2;
    }

    printf("EXPECTED EXCEPTION!!!\n");
    return 3;
}

template <typename Vector>
void CheckVector(const size_t expectedSize, Vector& vec)
{
    // needed for checking const methods alongside with non-const
    const Vector& cvec = vec;

    CHECK(expectedSize == vec.size());

    if (expectedSize == 0)
    {
        CHECK(vec.begin() == vec.end());
        CHECK(cvec.begin() == cvec.end());
        CHECK(vec.cbegin() == vec.cend());

        CHECK(vec.rbegin() == vec.rend());
        CHECK(cvec.rbegin() == cvec.rend());
        CHECK(vec.crbegin() == vec.crend());

        CHECK(false == vec.empty()); //true
    }
    else
    {
        CHECK(vec.begin() != vec.end());
        CHECK(cvec.begin() != cvec.end());
        CHECK(vec.cbegin() != vec.cend());

        CHECK(vec.rbegin() != vec.rend());
        CHECK(cvec.rbegin() != cvec.rend());
        CHECK(vec.crbegin() != vec.crend());

        CHECK(false == vec.empty());
    }

    CHECK(expectedSize == (vec.end() - vec.begin()));
    CHECK(expectedSize == (cvec.end() - cvec.begin()));
    CHECK(expectedSize == (vec.cend() - vec.cbegin()));

    CHECK(expectedSize == (vec.rend() - vec.rbegin()));
    CHECK(expectedSize == (cvec.rend() - cvec.rbegin()));
    CHECK(expectedSize == (vec.crend() - vec.crbegin()));
}

template <typename Type, typename Vector>
void CheckValueAtPosition(const Type& expectedValue, Vector& vec, size_t pos)
{
    const Vector& cvec = vec;
    CHECK(vec.size() > pos);
    CHECK(expectedValue == vec[pos]);
    CHECK(expectedValue == cvec[pos]);

    CHECK(expectedValue == vec.at(pos));
    CHECK(expectedValue == cvec.at(pos));

    CHECK(expectedValue == *(vec.begin() + pos));
    CHECK(expectedValue == *(cvec.begin() + pos));
    CHECK(expectedValue == *(vec.cbegin() + pos));
}

void TestDefaultCtor()
{
    std::vector<int> vec;
    CheckVector(0, vec);
}

void TestCtorWithAllocator()
{
    std::allocator<int> a;
    std::vector<int> vec(a);
    CheckVector(0, vec);
}

void TestCtorWithValueAndCount()
{
    std::vector<int> vec(10, 42);
    CheckVector(10, vec);

    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(42, vec, pos);
    }
}


void TestCtorWithCount()
{
    std::vector<int> vec(10);
    CheckVector(10, vec);

    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(0, vec, pos);
    }
}

void TestCtorWithInputIterator()
{
    int buf[] = {0, 1, 2, 3, 4, 5};
    std::vector<int> vec(buf, buf + 6);
    CheckVector(6, vec);

    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}

void TestCopyCtor()
{
    int buf [] = {0, 1, 2, 3, 4, 5};
    std::vector<int> input(buf, buf + 6);
    CheckVector(6, input);

    std::vector<int> vec(input);
    CheckVector(6, vec);
    CheckVector(6, input);

    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}

void TestCopyCtorWithAllocator()
{
    int buf [] = {0, 1, 2, 3, 4, 5};
    std::vector<int> input(buf, buf + 6);
    CheckVector(6, input);

    std::allocator<int> a;

    std::vector<int> vec(input, a);
    CheckVector(6, vec);
    CheckVector(6, input);

    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}


void TestMoveCtor()
{
    int buf [] = {0, 1, 2, 3, 4, 5};
    std::vector<int> input(buf, buf + 6);
    CheckVector(6, input);

    std::vector<int> vec(std::move(input));
    CheckVector(6, vec);
    CheckVector(0, input);

    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}

void TestMoveCtorWithAllocator()
{
    int buf [] = {0, 1, 2, 3, 4, 5};
    std::vector<int> input(buf, buf + 6);
    CheckVector(6, input);

    std::allocator<int> a;

    std::vector<int> vec(std::move(input), a);
    CheckVector(6, vec);
    CheckVector(0, input);

    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}


void TestVectorCtor()
{
    TestDefaultCtor();
    TestCtorWithAllocator();
    TestCtorWithValueAndCount();
    TestCtorWithCount();
    TestCtorWithInputIterator();
    TestCopyCtor();
    TestCopyCtorWithAllocator();
    TestMoveCtor();
    TestMoveCtorWithAllocator();
}




void TestResizeWithCount()
{
    std::vector<int> vec;

    vec.resize(10);
    CheckVector(10, vec);
    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(0, vec, pos);
    }

    vec.resize(20);
    CheckVector(10, vec);
    for (size_t pos = 0; pos != 20; ++pos)
    {
        CheckValueAtPosition(0, vec, pos);
    }

    vec.resize(5);
    CheckVector(10, vec);
    for (size_t pos = 0; pos != 20; ++pos)
    {
        CheckValueAtPosition(0, vec, pos);
    }
}


void TestResizeWithCountAndValue()
{
    std::vector<int> vec;

    vec.resize(10, 42);
    CheckVector(10, vec);
    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(42, vec, pos);
    }

    vec.resize(20, 43);
    CheckVector(20, vec);
    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(42, vec, pos);
    }
    for (size_t pos = 10; pos != 20; ++pos)
    {
        CheckValueAtPosition(43, vec, pos);
    }

    vec.resize(5, 44);
    CheckVector(5, vec);
    for (size_t pos = 0; pos != 5; ++pos)
    {
        CheckValueAtPosition(42, vec, pos);
    }
}

void TestResize()
{
    TestResizeWithCount();
    TestResizeWithCountAndValue();
}

int main()
{
    try
    {
        TestVectorCtor();
        TestResize();
    }
    catch(...)
    {
        return ProcessException();
    }

    return 0;
}
