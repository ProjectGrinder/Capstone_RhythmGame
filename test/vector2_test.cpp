#include "pch.h"
#include "maths/vector2.h"

TEST(Vector2, default_constructor_test)
{
    Math::Vector2<int> vector;
    EXPECT_EQ(vector.x, 0);
    EXPECT_EQ(vector.y, 0);
}

TEST(Vector2, parameterized_constructor_test)
{
    Math::Vector2<int> vector(3, 4);
    EXPECT_EQ(vector.x, 3);
    EXPECT_EQ(vector.y, 4);
}

TEST(Vector2, copy_constructor_test)
{
    Math::Vector2<int> original_vector(5, 6);
    Math::Vector2<int> copied_vector(original_vector);
    EXPECT_EQ(copied_vector.x, 5);
    EXPECT_EQ(copied_vector.y, 6);
}

TEST(Vector2, addition_operator_test)
{
    Math::Vector2<int> vector1(1, 2);
    Math::Vector2<int> vector2(3, 4);
    Math::Vector2<int> result = vector1 + vector2;
    EXPECT_EQ(result.x, 4);
    EXPECT_EQ(result.y, 6);
}

TEST(Vector2, subtraction_operator_test)
{
    Math::Vector2<int> vector1(5, 6);
    Math::Vector2<int> vector2(3, 4);
    Math::Vector2<int> result = vector1 - vector2;
    EXPECT_EQ(result.x, 2);
    EXPECT_EQ(result.y, 2);
}

TEST(Vector2, multiplication_operator_test)
{
    Math::Vector2<int> vector(2, 3);
    int scalar = 4;
    Math::Vector2<int> result = vector * scalar;
    EXPECT_EQ(result.x, 8);
    EXPECT_EQ(result.y, 12);
}

TEST(Vector2, division_operator_test)
{
    Math::Vector2<int> vector(8, 12);
    int scalar = 4;
    Math::Vector2<int> result = vector / scalar;
    EXPECT_EQ(result.x, 2);
    EXPECT_EQ(result.y, 3);
}

TEST(Vector2, division_by_zero_test)
{
    Math::Vector2<int> vector(8, 12);
    int scalar = 0;
    EXPECT_THROW(vector / scalar, std::runtime_error);
}

TEST(Vector2, equality_test)
{
    Math::Vector2<int> vector1(1, 2);
    Math::Vector2<int> vector2(1, 2);
    Math::Vector2<int> vector3(3, 4);
    EXPECT_TRUE(vector1 == vector1);
    EXPECT_TRUE(vector1 == vector2);
    EXPECT_FALSE(vector1 == vector3);
}