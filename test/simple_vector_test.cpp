#include <vector.h>

#include <stdio.h>

#define STR(x) #x
#define STR2(x) STR(x)
#define CHECK(val) do { if (!(val)) throw STR(val) " at " STR2(__FILE__) "(" STR2(__LINE__) ")";} while(0)



struct ObjectWithGlobalRefCount
{
    ObjectWithGlobalRefCount()
        : m_value()
    {
        ++objectCount;
    }

    ObjectWithGlobalRefCount(int value)
        : m_value(value)
    {
        ++objectCount;
    }

    ObjectWithGlobalRefCount(const ObjectWithGlobalRefCount& other)
        : m_value(other.m_value)
    {
        ++objectCount;
    }

    ObjectWithGlobalRefCount(ObjectWithGlobalRefCount&& other)
        : m_value(std::move(other.m_value))
    {
        ++objectCount;
    }

    ~ObjectWithGlobalRefCount()
    {
        --objectCount;
    }

    ObjectWithGlobalRefCount& operator=(const ObjectWithGlobalRefCount& other)
    {
        if (&other != this)
        {
            m_value = other.m_value;
        }

        return *this;
    }

    ObjectWithGlobalRefCount& operator=(ObjectWithGlobalRefCount&& other)
    {
        if (&other != this)
        {
            m_value = std::move(other.m_value);
        }

        return *this;
    }

    int m_value;

    static size_t objectCount;
};

inline bool operator==(const ObjectWithGlobalRefCount& left, int right)
{
    return left.m_value == right;
}

inline bool operator==(int left, const ObjectWithGlobalRefCount& right)
{
    return left == right.m_value;
}

size_t ObjectWithGlobalRefCount::objectCount = 0;


#define RUN_TEST(test) do {printf("running test %s...", STR(test)); test; CHECK((0 == ObjectWithGlobalRefCount::objectCount) && STR(test) " failed"); printf(" done\n");} while(0)
#define RUN_TEST_SUITE(test) do {printf("running test suite %s...\n", STR(test)); test; printf("done\n\n");} while(0)

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




template <typename Type>
void TestDefaultCtor()
{
    std::vector<Type> vec;
    CheckVector(0, vec);
}

template <typename Type>
void TestCtorWithAllocator()
{
    std::allocator<Type> a;
    std::vector<Type> vec(a);
    CheckVector(0, vec);
}

template <typename Type>
void TestCtorWithValueAndCount()
{
    {
        size_t count = 0;
        Type value = 0;
        std::vector<Type> vec(count, value);
        CheckVector(0, vec);
    }

    {
        std::vector<Type> vec(10, 42);
        CheckVector(10, vec);

        for (size_t pos = 0; pos != 10; ++pos)
        {
            CheckValueAtPosition(42, vec, pos);
        }
    }
}

template <typename Type>
void TestCtorWithCount()
{
    {
        size_t count = 0;
        std::vector<Type> vec(count);
        CheckVector(0, vec);
    }

    {
        std::vector<Type> vec(10);
        CheckVector(10, vec);

        for (size_t pos = 0; pos != 10; ++pos)
        {
            CheckValueAtPosition(0, vec, pos);
        }
    }
}

template <typename Type>
void TestCtorWithInputIterator()
{
    {
        int buf [] = {0, 1, 2, 3, 4, 5};
        std::vector<Type> vec(buf, buf);
        CheckVector(0, vec);
    }
    {
        int buf [] = {0, 1, 2, 3, 4, 5};
        std::vector<Type> vec(buf, buf + 6);
        CheckVector(6, vec);

        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec, pos);
        }
    }
}

template <typename Type>
void TestCopyCtor()
{
    int buf [] = {0, 1, 2, 3, 4, 5};
    std::vector<Type> input(buf, buf + 6);
    CheckVector(6, input);

    std::vector<Type> vec(input);
    CheckVector(6, vec);
    CheckVector(6, input);

    for (size_t pos = 0; pos != 6; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}

template <typename Type>
void TestCopyCtorWithAllocator()
{
    int buf [] = {0, 1, 2, 3, 4, 5};
    std::vector<Type> input(buf, buf + 6);
    CheckVector(6, input);

    std::allocator<Type> a;

    std::vector<Type> vec(input, a);
    CheckVector(6, vec);
    CheckVector(6, input);

    for (size_t pos = 0; pos != 6; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}

template <typename Type>
void TestMoveCtor()
{
    int buf [] = {0, 1, 2, 3, 4, 5};
    std::vector<Type> input(buf, buf + 6);
    CheckVector(6, input);

    std::vector<Type> vec(std::move(input));
    CheckVector(6, vec);
    CheckVector(0, input);

    for (size_t pos = 0; pos != 6; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}

template <typename Type>
void TestMoveCtorWithAllocator()
{
    int buf [] = {0, 1, 2, 3, 4, 5};
    std::vector<Type> input(buf, buf + 6);
    CheckVector(6, input);

    std::allocator<Type> a;

    std::vector<Type> vec(std::move(input), a);
    CheckVector(6, vec);
    CheckVector(0, input);

    for (size_t pos = 0; pos != 6; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}

template <typename Type>
void TestCtorWithInitializerList()
{
    std::vector<Type> vec({0, 1, 2, 3, 4, 5});
    CheckVector(6, vec);

    for (size_t pos = 0; pos != 6; ++pos)
    {
        CheckValueAtPosition(static_cast<int>(pos), vec, pos);
    }
}

template <typename Type>
void TestVectorCtor()
{
    RUN_TEST(TestDefaultCtor<Type>());
    RUN_TEST(TestCtorWithAllocator<Type>());
    RUN_TEST(TestCtorWithValueAndCount<Type>());
    RUN_TEST(TestCtorWithCount<Type>());
    RUN_TEST(TestCtorWithInputIterator<Type>());
    RUN_TEST(TestCopyCtor<Type>());
    RUN_TEST(TestCopyCtorWithAllocator<Type>());
    RUN_TEST(TestMoveCtor<Type>());
    RUN_TEST(TestMoveCtorWithAllocator<Type>());
    RUN_TEST(TestCtorWithInitializerList<Type>());
}


template <typename Type>
void TestCopyAssignment()
{
    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        std::vector<Type> vec2;
        vec2 = vec;
        CheckVector(6, vec2);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec2, pos);
            CheckValueAtPosition(static_cast<int>(pos), vec, pos);
        }
    }

    {
        std::vector<Type> vec;

        std::vector<Type> vec2;
        vec2 = vec;
        CheckVector(0, vec2);
    }

    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        std::vector<Type> vec2({6, 7, 8, 9});
        vec2 = vec;
        CheckVector(6, vec2);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec2, pos);
            CheckValueAtPosition(static_cast<int>(pos), vec, pos);
        }
    }
}

template <typename Type>
void TestMoveAssignment()
{
    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        std::vector<Type> vec2;
        vec2 = std::move(vec);
        CheckVector(6, vec2);
        CheckVector(0, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec2, pos);
        }
    }

    {
        std::vector<Type> vec;

        std::vector<Type> vec2;
        vec2 = std::move(vec);
        CheckVector(0, vec2);
        CheckVector(0, vec);
    }

    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        std::vector<Type> vec2({6, 7, 8, 9});
        vec2 = std::move(vec);
        CheckVector(6, vec2);
        CheckVector(0, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec2, pos);
        }
    }
}

template <typename Type>
void TestInitializerListAssignment()
{
    {
        std::vector<Type> vec;
        vec = {0, 1, 2, 3, 4, 5};
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(static_cast<int>(pos), vec, pos);
        }
    }
}


template <typename Type>
void TestAssignmentOperator()
{
    RUN_TEST(TestCopyAssignment<Type>());
    RUN_TEST(TestMoveAssignment<Type>());
    RUN_TEST(TestInitializerListAssignment<Type>());
}


template <typename Type>
void TestAssignWithCountAndValue()
{
    {
        std::vector<Type> vec;

        vec.assign(5, 42);
        CheckVector(5, vec);
        for (size_t pos = 0; pos != 5; ++pos)
        {
            CheckValueAtPosition(42, vec, pos);
        }
    }

    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        vec.assign(0, 42);
        CheckVector(0, vec);
    }

    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        vec.assign(5, 42);
        CheckVector(5, vec);
        for (size_t pos = 0; pos != 5; ++pos)
        {
            CheckValueAtPosition(42, vec, pos);
        }
    }
}

template <typename Type>
void TestAssignWithRangeOfIterators()
{
    {
        std::vector<Type> vec;

        int buf [] = {0, 1, 2, 3, 4, 5};
        vec.assign(buf, buf + 6);
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(pos, vec, pos);
        }
    }

    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        int buf [] = {0, 1, 2, 3, 4, 5};
        vec.assign(buf, buf);
        CheckVector(0, vec);
    }

    {
        std::vector<Type> vec({6, 7, 8, 9});

        int buf [] = {0, 1, 2, 3, 4, 5};
        vec.assign(buf, buf + 6);
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(pos, vec, pos);
        }
    }
}

template <typename Type>
void TestAssignWithInitializerList()
{
    {
        std::vector<Type> vec;

        vec.assign({0, 1, 2, 3, 4, 5});
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(pos, vec, pos);
        }
    }

    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        vec.assign({});
        CheckVector(0, vec);
    }

    {
        std::vector<Type> vec({6, 7, 8, 9});

        vec.assign({0, 1, 2, 3, 4, 5});
        CheckVector(6, vec);
        for (size_t pos = 0; pos != 6; ++pos)
        {
            CheckValueAtPosition(pos, vec, pos);
        }
    }
}


template <typename Type>
void TestAssignMethod()
{
    RUN_TEST(TestAssignWithCountAndValue<Type>());
    RUN_TEST(TestAssignWithRangeOfIterators<Type>());
    RUN_TEST(TestAssignWithInitializerList<Type>());
}


template <typename Type>
void TestResizeWithCount()
{
    std::vector<Type> vec;

    vec.resize(10);
    CheckVector(10, vec);
    for (size_t pos = 0; pos != 10; ++pos)
    {
        CheckValueAtPosition(0, vec, pos);
    }

    vec.resize(20);
    CheckVector(20, vec);
    for (size_t pos = 0; pos != 20; ++pos)
    {
        CheckValueAtPosition(0, vec, pos);
    }

    vec.resize(5);
    CheckVector(5, vec);
    for (size_t pos = 0; pos != 5; ++pos)
    {
        CheckValueAtPosition(0, vec, pos);
    }
}


template <typename Type>
void TestResizeWithCountAndValue()
{
    std::vector<Type> vec;

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

template <typename Type>
void TestResize()
{
    RUN_TEST(TestResizeWithCount<Type>());
    RUN_TEST(TestResizeWithCountAndValue<Type>());
}

template <typename Type>
void TestReserve()
{
    std::vector<Type> vec;

    vec.reserve(10);
    CheckVector(0, vec);
    CHECK(10 == vec.capacity());

    vec.reserve(9);
    CheckVector(0, vec);
    CHECK(10 == vec.capacity()); // capacity does not change
}

template <typename Type>
void TestEraseOneElement()
{
    std::vector<Type> vec({0, 1, 2, 3, 4, 5});

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



template <typename Type>
void TestEraseRange()
{
    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        auto res = vec.erase(vec.begin(), vec.end());
        CheckVector(0, vec);
        CHECK(vec.end() == res);
    }

    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        auto res = vec.erase(vec.begin(), vec.begin());
        CheckVector(6, vec);
        CHECK(vec.begin() == res);
        for (size_t i = 0; i != 6; ++i)
        {
            CheckValueAtPosition(i, vec, i);
        }
    }


    {
        std::vector<Type> vec({0, 1, 2, 3, 4, 5});

        auto res = vec.erase(vec.begin() + 1, vec.end() - 1);
        CheckVector(2, vec);
        CHECK(5 == *res);
        CheckValueAtPosition(0, vec, 0);
        CheckValueAtPosition(5, vec, 1);
    }
}


template <typename Type>
void TestErase()
{
    RUN_TEST(TestEraseOneElement<Type>());
    RUN_TEST(TestEraseRange<Type>());
}



template <typename Type>
void TestInsertAtPosWithCref()
{
    std::vector<Type> vec;

    const Type v10 = 10;
    auto res = vec.insert(vec.begin(), v10);
    CheckVector(1, vec);

    CheckValueAtPosition(10, vec, 0);
    CHECK(10 == *res);

    const Type v20 = 20;
    res = vec.insert(vec.end(), v20);
    CheckVector(2, vec);

    CheckValueAtPosition(10, vec, 0);
    CheckValueAtPosition(20, vec, 1);
    CHECK(20 == *res);
}


template <typename Type>
void TestInsertAtPosWithRvalueRef()
{
    std::vector<Type> vec;

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

template <typename Type>
void TestInsertAtPosWithCrefAndCount()
{
    std::vector<Type> vec;

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

template <typename Type>
void TestInsertAtPosWithRange()
{
    std::vector<Type> vec;

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
        CheckValueAtPosition(6, vec, 3);
        CheckValueAtPosition(7, vec, 4);
        CheckValueAtPosition(8, vec, 5);
        CheckValueAtPosition(9, vec, 6);
        CheckValueAtPosition(3, vec, 7);
        CheckValueAtPosition(4, vec, 8);
        CheckValueAtPosition(5, vec, 9);

        CHECK((vec.begin() + 3) == res);
    }
}


template <typename Type>
void TestInsert()
{
    RUN_TEST(TestInsertAtPosWithCref<Type>());
    RUN_TEST(TestInsertAtPosWithRvalueRef<Type>());
    RUN_TEST(TestInsertAtPosWithRange<Type>());
}

template <typename Type>
void TestShrinkToFit()
{
    std::vector<Type> vec;

    vec.shrink_to_fit();
    CheckVector(0, vec);
    CHECK(vec.size() == vec.capacity());

    vec.resize(5);
    vec.assign({0, 1, 2, 3, 4, 5});
    CHECK(vec.capacity() > vec.size()); // test precondition, update test if error

    vec.shrink_to_fit();
    CheckVector(6, vec);
    CHECK(vec.capacity() == vec.size());
    for (size_t i = 0; i != 6; ++i)
    {
        CheckValueAtPosition(i, vec, i);
    }
}

template <typename Type>
void TestClear()
{
    std::vector<Type> vec;
    size_t capacity = vec.capacity();
    vec.clear();
    CheckVector(0, vec);
    CHECK(capacity == vec.capacity());

    vec.resize(5);
    CheckVector(5, vec);
    capacity = vec.capacity();
    vec.clear();
    CheckVector(0, vec);
    CHECK(capacity == vec.capacity());

    capacity = vec.capacity();
    vec.clear();
    CheckVector(0, vec);
    CHECK(capacity == vec.capacity());
}

template <typename Type>
void TestPushBackCref()
{
    std::vector<Type> vec;

    const Type v10 = 10;
    vec.push_back(v10);
    CheckVector(1, vec);
    CheckValueAtPosition(10, vec, 0);

    const Type v20 = 20;
    vec.push_back(v20);
    CheckVector(2, vec);
    CheckValueAtPosition(10, vec, 0);
    CheckValueAtPosition(20, vec, 1);
}

template <typename Type>
void TestPushBackRvalueRef()
{
    std::vector<Type> vec;

    vec.push_back(10);
    CheckVector(1, vec);
    CheckValueAtPosition(10, vec, 0);

    vec.push_back(20);
    CheckVector(2, vec);
    CheckValueAtPosition(10, vec, 0);
    CheckValueAtPosition(20, vec, 1);
}

template <typename Type>
void TestPushBack()
{
    TestPushBackCref<Type>();
    TestPushBackRvalueRef<Type>();
}

template <typename Type>
void CompileTimeTests()
{
    typedef typename std::vector<Type>::allocator_type my_alloc;
}

template <typename Type>
void VectorTestSuites()
{
    RUN_TEST_SUITE(TestVectorCtor<Type>());
    RUN_TEST_SUITE(TestAssignmentOperator<Type>());
    RUN_TEST_SUITE(TestAssignMethod<Type>());
    RUN_TEST_SUITE(TestResize<Type>());
    RUN_TEST_SUITE(TestReserve<Type>());
    RUN_TEST_SUITE(TestErase<Type>());
    RUN_TEST_SUITE(TestInsert<Type>());
    RUN_TEST_SUITE(TestShrinkToFit<Type>());
    RUN_TEST_SUITE(TestClear<Type>());
    RUN_TEST_SUITE(TestPushBack<Type>());
    RUN_TEST_SUITE(CompileTimeTests<Type>());
}

int main()
{
    try
    {
        VectorTestSuites<ObjectWithGlobalRefCount>();
        VectorTestSuites<int>();
    }
    catch(...)
    {
        return ProcessException();
    }

    return 0;
}
