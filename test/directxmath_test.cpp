#include "pch.h"
#include <DirectXMath.h>

TEST(XMVECTOR, addition_operator_test)
{
    DirectX::XMINT2 vector1{1, 2};
    DirectX::XMINT2 vector2{3, 4};
    DirectX::XMVECTOR val1 = DirectX::XMLoadSInt2(&vector1);
    DirectX::XMVECTOR val2 = DirectX::XMLoadSInt2(&vector2);
    DirectX::XMVECTOR result = DirectX::XMVectorAdd(val1, val2);
    DirectX::XMINT2 result_vector{};
    DirectX::XMStoreSInt2(&result_vector, result);
    EXPECT_EQ(result_vector.x, 4);
    EXPECT_EQ(result_vector.y, 6);
}

TEST(XMVECTOR, subtraction_operator_test)
{
    DirectX::XMINT2 vector1{5, 6};
    DirectX::XMINT2 vector2{3, 4};
    DirectX::XMVECTOR val1 = DirectX::XMLoadSInt2(&vector1);
    DirectX::XMVECTOR val2 = DirectX::XMLoadSInt2(&vector2);
    DirectX::XMVECTOR result = DirectX::XMVectorSubtract(val1, val2);
    DirectX::XMINT2 result_vector{};
    DirectX::XMStoreSInt2(&result_vector, result);
    EXPECT_EQ(result_vector.x, 2);
    EXPECT_EQ(result_vector.y, 2);
}

TEST(XMVECTOR, multiplication_operator_test)
{
    DirectX::XMINT2 vector(2, 3);
    float scalar = 4;
    DirectX::XMVECTOR val = DirectX::XMLoadSInt2(&vector);
    DirectX::XMVECTOR scalarVector = DirectX::XMVectorReplicate(scalar);
    DirectX::XMVECTOR result = DirectX::XMVectorMultiply(val, scalarVector);
    DirectX::XMINT2 result_vector{};
    DirectX::XMStoreSInt2(&result_vector, result);
    EXPECT_EQ(result_vector.x, 8);
    EXPECT_EQ(result_vector.y, 12);
}

TEST(XMVECTOR, division_operator_test)
{
    DirectX::XMINT2 vector(8, 12);
    float scalar = 4;
    DirectX::XMVECTOR val = DirectX::XMLoadSInt2(&vector);
    DirectX::XMVECTOR scalarVector = DirectX::XMVectorReplicate(scalar);
    DirectX::XMVECTOR result = DirectX::XMVectorDivide(val, scalarVector);
    DirectX::XMINT2 result_vector{};
    DirectX::XMStoreSInt2(&result_vector, result);
    EXPECT_EQ(result_vector.x, 2);
    EXPECT_EQ(result_vector.y, 3);
}

TEST(XMVECTOR, equality_test)
{
    DirectX::XMINT2 vector1(1, 2);
    DirectX::XMINT2 vector2(1, 2);
    DirectX::XMINT2 vector3(3, 4);
    DirectX::XMVECTOR val1 = DirectX::XMLoadSInt2(&vector1);
    DirectX::XMVECTOR val2 = DirectX::XMLoadSInt2(&vector2);
    DirectX::XMVECTOR val3 = DirectX::XMLoadSInt2(&vector3);
    EXPECT_TRUE(DirectX::XMVector4Equal(val1, val1));
    EXPECT_TRUE(DirectX::XMVector4Equal(val1, val2));
    EXPECT_FALSE(DirectX::XMVector4Equal(val1, val3));
}