#include "BigIntegerDB.h"

#include "gtest/gtest.h"

//
// Tests adding two 4-byte BigIntegers together.
//
TEST(BigIntegerDBTest, Addition)
{
	const BigIntegerDB<4> addend1 = BigIntegerDB<4>::FromHex("0x01020309");
	const BigIntegerDB<4> addend2 = BigIntegerDB<4>::FromHex("0x020304F8");

	const BigIntegerDB<4> actualSum = addend1 + addend2;
	const BigIntegerDB<4> expectedSum = BigIntegerDB<4>::FromHex("0x03050801");

	EXPECT_EQ(actualSum, expectedSum);
}

//
// Tests subtracting a 4-byte BigInteger from a larger one.
//
TEST(BigIntegerDBTest, Subtraction)
{
	const BigIntegerDB<4> minuend = BigIntegerDB<4>::FromHex("0x03050801");
	const BigIntegerDB<4> subtrahend = BigIntegerDB<4>::FromHex("0x01020309");

	const BigIntegerDB<4> actualDifference = minuend - subtrahend;
	const BigIntegerDB<4> expectedDifference = BigIntegerDB<4>::FromHex("0x020304F8");

	EXPECT_EQ(actualDifference, expectedDifference);
}

//
// Tests multiplying two 4-byte BigIntegers together.
//
TEST(BigIntegerDBTest, Multiplication)
{
	const BigIntegerDB<4> multiplicand = BigIntegerDB<4>::FromHex("0x00001801");
	const BigIntegerDB<4> multiplier = BigIntegerDB<4>::FromHex("0x00001309");

	const BigIntegerDB<4> actualProduct = multiplicand * multiplier;
	const BigIntegerDB<4> expectedProduct = BigIntegerDB<4>::FromHex("0x01C8EB09");

	EXPECT_EQ(actualProduct, expectedProduct);
}

//
// Tests dividing a 4-byte BigInteger from a larger one.
//
TEST(BigIntegerDBTest, Division)
{
	const BigIntegerDB<4> dividend = BigIntegerDB<4>::FromHex("0x01C8EB09");
	const BigIntegerDB<4> divisor = BigIntegerDB<4>::FromHex("0x00001801");

	const BigIntegerDB<4> actualQuotient = dividend / divisor;
	const BigIntegerDB<4> expectedQuotient = BigIntegerDB<4>::FromHex("0x00001309");

	EXPECT_EQ(actualQuotient, expectedQuotient);
}

//
// Tests dividing a 4-byte BigInteger from a larger one and calculating the modulus.
//
TEST(BigIntegerDBTest, Modulus)
{
	const BigIntegerDB<4> dividend = BigIntegerDB<4>::FromHex("0x01C8EC0B");
	const BigIntegerDB<4> divisor = BigIntegerDB<4>::FromHex("0x00001801");

	const BigIntegerDB<4> actualMod = dividend % divisor;
	const BigIntegerDB<4> expectedMod = BigIntegerDB<4>::FromHex("0x00000102");

	EXPECT_EQ(actualMod, expectedMod);
}

//
// Tests converting various sized primitives into BigIntegers.
//
TEST(BigIntegerDBTest, ValueOf)
{
	uint64_t eightByteNumber = 35954775201473703L; // 0x007F BCAD 73DC E4A7
	const BigIntegerDB<1> actual1 = BigIntegerDB<1>::ValueOf(eightByteNumber);
	const BigIntegerDB<1> expected1 = BigIntegerDB<1>::FromHex("0xA7");
	EXPECT_EQ(actual1, expected1);

	const BigIntegerDB<2> actual2 = BigIntegerDB<2>::ValueOf(eightByteNumber);
	const BigIntegerDB<2> expected2 = BigIntegerDB<2>::FromHex("0xE4A7");
	EXPECT_EQ(actual2, expected2);

	const BigIntegerDB<4> actual4 = BigIntegerDB<4>::ValueOf(eightByteNumber);
	const BigIntegerDB<4> expected4 = BigIntegerDB<4>::FromHex("0x73DCE4A7");
	EXPECT_EQ(actual4, expected4);

	const BigIntegerDB<8> actual8 = BigIntegerDB<8>::ValueOf(eightByteNumber);
	const BigIntegerDB<8> expected8 = BigIntegerDB<8>::FromHex("0x007FBCAD73DCE4A7");
	EXPECT_EQ(actual8, expected8);
}