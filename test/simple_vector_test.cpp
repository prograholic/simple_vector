#include <vector.h>

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
    CHECK(expectedSize <= vec.capacity());

    if (expectedSize == 0)
    {
        CHECK(vec.begin() == vec.end());
        CHECK(cvec.begin() == cvec.end());
        CHECK(vec.cbegin() == vec.cend());

        CHECK(vec.rbegin() == vec.rend());
        CHECK(cvec.rbegin() == cvec.rend());
        CHECK(vec.crbegin() == vec.crend());

        CHECK(true == vec.empty());
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

void TestCtorWithInitializerList()
{
    std::vector<int> vec({0, 1, 2, 3, 4, 5});
    CheckVector(6, vec);

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
    TestCtorWithInitializerList();
}



void TestCopyAssignment()
{
    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        std::vector<int> vec2;
        vec2 = vec;
        CheckVector(6, vec2);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec2, pos);
            CheckValueAtPosition(static_cast<int>(pos), vec, pos);
        }
    }

    {
        std::vector<int> vec;

        std::vector<int> vec2;
        vec2 = vec;
        CheckVector(0, vec2);
    }

    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        std::vector<int> vec2({6, 7, 8, 9});
        vec2 = vec;
        CheckVector(6, vec2);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec2, pos);
            CheckValueAtPosition(static_cast<int>(pos), vec, pos);
        }
    }
}

void TestMoveAssignment()
{
    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        std::vector<int> vec2;
        vec2 = std::move(vec);
        CheckVector(6, vec2);
        CheckVector(0, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec, pos);
        }
    }

    {
        std::vector<int> vec;

        std::vector<int> vec2;
        vec2 = std::move(vec);
        CheckVector(0, vec2);
        CheckVector(0, vec);
    }

    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        std::vector<int> vec2({6, 7, 8, 9});
        vec2 = std::move(vec);
        CheckVector(6, vec2);
        CheckVector(0, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec2, pos);
        }
    }
}

void TestInitializerListAssignment()
{
    {
        std::vector<int> vec;
        vec = {0, 1, 2, 3, 4, 5};
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec, pos);
        }
    }
}


void TestAssignmentOperator()
{
    TestCopyAssignment();
    TestMoveAssignment();
    TestInitializerListAssignment();
}


void TestAssignWithCountAndValue()
{
    {
        std::vector<int> vec;

        vec.assign(5, 42);
        CheckVector(5, vec);
        for (size_t pos = 0; pos != 5; ++pos)
        {
            CheckValueAtPosition(42, vec, pos);
        }
    }

    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        vec.assign(0, 42);
        CheckVector(0, vec);
    }

    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        vec.assign(5, 42);
        CheckVector(5, vec);
        for (size_t pos = 0; pos != 5; ++pos)
        {
            CheckValueAtPosition(42, vec, pos);
        }
    }
}

void TestAssignWithRangeOfIterators()
{
    {
        std::vector<int> vec;

        int buf [] = {0, 1, 2, 3, 4, 5};
        vec.assign(buf, buf + 6);
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(pos, vec, pos);
        }
    }

    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        int buf [] = {0, 1, 2, 3, 4, 5};
        vec.assign(buf, buf);
        CheckVector(0, vec);
    }

    {
        std::vector<int> vec({6, 7, 8, 9});

        int buf [] = {0, 1, 2, 3, 4, 5};
        vec.assign(buf, buf + 6);
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(pos, vec, pos);
        }
    }
}

void TestAssignWithInitializerList()
{
    {
        std::vector<int> vec;

        vec.assign({0, 1, 2, 3, 4, 5});
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(pos, vec, pos);
        }
    }

    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        vec.assign({});
        CheckVector(0, vec);
    }

    {
        std::vector<int> vec({6, 7, 8, 9});

        vec.assign({0, 1, 2, 3, 4, 5});
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(pos, vec, pos);
        }
    }
}


void TestAssignMethod()
{
    TestAssignWithCountAndValue();
    TestAssignWithRangeOfIterators();
    TestAssignWithInitializerList();
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

void TestReserve()
{
    std::vector<int> vec;

    vec.reserve(10);
    CheckVector(0, vec);
    CHECK(10 == vec.capacity());

    vec.reserve(9);
    CheckVector(0, vec);
    CHECK(10 == vec.capacity()); // capacity does not change
}

void TestEraseOneElement()
{
    std::vector<int> vec({0, 1, 2, 3, 4, 5});

    auto res = vec.erase(vec.begin());
    CheckVector(5, vec);
    CHECK(1 == *res);
    for (size_t i = 0; i != 5; ++i)
    {
        CheckValueAtPosition(i + 1, vec, i);
    }


    res = vec.erase(vec.end() - 1);
    CheckVector(4, vec);
    CHECK(vec.end() == res);
    for (size_t i = 0; i != 4; ++i)
    {
        CheckValueAtPosition(i + 1, vec, i);
    }


    res = vec.erase(vec.end() - 2);
    CheckVector(3, vec);
    CHECK(4 == *res);
    CheckValueAtPosition(1, vec, 0);
    CheckValueAtPosition(2, vec, 1);
    CheckValueAtPosition(4, vec, 2);
}



void TestEraseRange()
{
    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        auto res = vec.erase(vec.begin(), vec.end());
        CheckVector(0, vec);
        CHECK(vec.end() == res);
    }

    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        auto res = vec.erase(vec.begin(), vec.begin());
        CheckVector(6, vec);
        CHECK(vec.begin() == res);
        for (size_t i = 0; i != 6; ++i)
        {
            CheckValueAtPosition(i, vec, i);
        }
    }


    {
        std::vector<int> vec({0, 1, 2, 3, 4, 5});

        auto res = vec.erase(vec.begin() + 1, vec.end() - 1);
        CheckVector(2, vec);
        CHECK(5 == *res);
        CheckValueAtPosition(0, vec, 0);
        CheckValueAtPosition(5, vec, 1);
    }
}


void TestErase()
{
    TestEraseOneElement();
    TestEraseRange();
}



void TestInsertAtPosWithCref()
{
    std::vector<int> vec;

    auto res = vec.insert(vec.begin(), 10);
    CheckVector(1, vec);

    CheckValueAtPosition(10, vec, 0);
    CHECK(10 == *res);

    res = vec.insert(vec.end(), 20);
    CheckVector(2, vec);

    CheckValueAtPosition(10, vec, 0);
    CheckValueAtPosition(20, vec, 1);
    CHECK(20 == *res);
}


void TestInsertAtPosWithRvalueRef()
{
    std::vector<int> vec;

    auto res = vec.insert(vec.begin(), std::move(10));
    CheckVector(1, vec);

    CheckValueAtPosition(10, vec, 0);
    CHECK(10 == *res);

    res = vec.insert(vec.end(), std::move(20));
    CheckVector(2, vec);

    CheckValueAtPosition(10, vec, 0);
    CheckValueAtPosition(20, vec, 1);
    CHECK(20 == *res);
}

void TestInsertAtPosWithCrefAndCount()
{
    std::vector<int> vec;

    auto res = vec.insert(vec.begin(), 5, 10);
    CheckVector(5, vec);
    for (size_t i = 0; i != 5; ++i)
    {
        CheckValueAtPosition(10, vec, i);
    }
    CHECK(10 == *res);

    res = vec.insert(vec.end(), 0, 20);
    CheckVector(5, vec);
    for (size_t i = 0; i != 5; ++i)
    {
        CheckValueAtPosition(10, vec, i);
    }
    CHECK(vec.end() == res);
}

void TestInsertAtPosWithRange()
{
    std::vector<int> vec;

    {
        int buf [] = {0, 1, 2, 3, 4, 5};
        auto res = vec.insert(vec.begin(), buf, buf + 6);
        CheckVector(6, vec);
        for (size_t i = 0; i != 6; ++i)
        {
            CheckValueAtPosition(i, vec, i);
        }
        CHECK(vec.begin() == res);
    }

    {
        int buf [] = {0, 1, 2, 3, 4, 5};
        auto res = vec.insert(vec.begin() + 3, buf, buf);
        CheckVector(6, vec);
        for (size_t i = 0; i != 6; ++i)
        {
            CheckValueAtPosition(i, vec, i);
        }
        CHECK((vec.begin() + 3) == res);
    }

    {
        int buf [] = {6, 7, 8, 9};
        auto res = vec.insert(vec.begin() + 3, buf, buf + 4);
        CheckVector(10, vec);
        CheckValueAtPosition(0, vec, 0);
        CheckValueAtPosition(1, vec, 1);
        CheckValueAtPosition(2, vec, 2);
        CheckValueAtPosition(3, vec, 3);
        CheckValueAtPosition(6, vec, 4);
        CheckValueAtPosition(7, vec, 5);
        CheckValueAtPosition(8, vec, 6);
        CheckValueAtPosition(9, vec, 7);
        CheckValueAtPosition(4, vec, 8);
        CheckValueAtPosition(5, vec, 9);

        CHECK((vec.begin() + 3) == res);
    }
}


void TestInsert()
{
    TestInsertAtPosWithCref();
    TestInsertAtPosWithRvalueRef();
    TestInsertAtPosWithRange();
}

int main()
{
    try
    {
        TestVectorCtor();
        TestAssignmentOperator();
        TestAssignMethod();
        TestResize();
        TestReserve();
        TestErase();
        TestInsert();
    }
    catch(...)
    {
        return ProcessException();
    }

    return 0;
}
