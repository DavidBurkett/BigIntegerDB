#pragma once

//
// This code is free for all purposes without any express guarantee it works.
//
// Author: David Burkett (davidburkett38@gmail.com)
//

#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <stdint.h>

//
// BigIntegerDB is a fixed-width BigInteger class that allows type-safety when needed.
// This is especially useful for cryptography, where inputs and outputs are of an expected size.
// For example, a SHA256 method would return a BigIntegerDB<32> object, notifying the consumer that the output is 32 bytes (256 bits).
//
// NOTE: This only supports unsigned BigIntegers, due to its intention of being used for cryptographic-type applications.
//
template<size_t NUM_BYTES>
class BigIntegerDB
{
public:
	//
	// Default constructor constructs a BigIntegerDB with the value of Zero.
	//
	BigIntegerDB()
		: m_data(NUM_BYTES)
	{
	}

	//
	// Initializes a BigIntegerDB with the value pointed to by the given byte array.
	// NOTE: The given byte array MUST be the length of NUM_BYTES.
	//
	BigIntegerDB(const uint8_t* data)
		: m_data(data, data + NUM_BYTES)
	{
	}

	//
	// Copy constructor.
	//
	BigIntegerDB(const BigIntegerDB& bigInteger)
	{
		m_data = bigInteger.GetData();
	}

	~BigIntegerDB() = default;

	static BigIntegerDB<NUM_BYTES> ValueOf(const uint64_t value);
	static BigIntegerDB<NUM_BYTES> FromHex(const std::string& hex);
	static BigIntegerDB<NUM_BYTES> GetMaximumValue();

	BigIntegerDB operator+(const int addend) const;
	BigIntegerDB operator+(const BigIntegerDB& addend) const;

	BigIntegerDB operator-(const int subtrahend) const;
	BigIntegerDB operator-(const BigIntegerDB& subtrahend) const;

	BigIntegerDB operator*(const int multiplier) const;
	BigIntegerDB operator*(const BigIntegerDB& A) const;

	BigIntegerDB operator/(const int divisor) const;
	BigIntegerDB operator/(const BigIntegerDB& divisor) const;

	int operator%(const int modulo) const;
	BigIntegerDB operator%(const BigIntegerDB& modulo) const;

	inline const std::vector<uint8_t>& ToVector() const { return m_data; }
	inline const uint8_t* ToCharArray() const { return &m_data[0]; }
	inline uint8_t& operator[] (const int x) { return m_data[x]; }
	inline const uint8_t& operator[] (const int x) const { return m_data[x]; }

	inline bool operator<(const BigIntegerDB& rhs) const
	{
		for (size_t i = 0; i < NUM_BYTES; i++)
		{
			if (this->m_data[i] < rhs.m_data[i])
			{
				return true;
			}
			else if (this->m_data[i] > rhs.m_data[i])
			{
				return false;
			}
		}

		return false;
	}

	inline bool operator>(const BigIntegerDB& rhs) const
	{
		return rhs < *this;
	}

	inline bool operator==(const BigIntegerDB& rhs) const
	{
		for (size_t i = 0; i < NUM_BYTES; i++)
		{
			if (this->m_data[i] != rhs.m_data[i])
			{
				return false;
			}
		}

		return true;
	}

	inline bool operator!=(const BigIntegerDB& rhs) const
	{
		return !(*this == rhs);
	}

	inline bool operator<=(const BigIntegerDB& rhs) const
	{
		return *this < rhs || *this == rhs;
	}

	inline bool operator>=(const BigIntegerDB& rhs) const
	{
		return *this > rhs || *this == rhs;
	}

	inline BigIntegerDB operator+=(const BigIntegerDB& rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	inline BigIntegerDB operator-=(const BigIntegerDB& rhs)
	{
		*this = *this - rhs;
		return *this;
	}

	inline BigIntegerDB operator*=(const BigIntegerDB& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	inline BigIntegerDB operator/=(const BigIntegerDB& rhs)
	{
		*this = *this / rhs;
		return *this;
	}

private:
	std::vector<uint8_t> m_data;
};

// TODO: There's definitiely an easier way to do this :-)
template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::ValueOf(const uint64_t value)
{
	std::vector<uint8_t> data(NUM_BYTES);

	data[NUM_BYTES - 1] = value & 0xff;

	if (NUM_BYTES >= 2)
	{
		data[NUM_BYTES - 2] = (value >> 8) & 0xff;
	}

	if (NUM_BYTES >= 4)
	{
		data[NUM_BYTES - 3] = (value >> 16) & 0xff;
		data[NUM_BYTES - 4] = (value >> 24) & 0xff;
	}

	if (NUM_BYTES >= 8)
	{
		data[NUM_BYTES - 5] = (value >> 32) & 0xff;
		data[NUM_BYTES - 6] = (value >> 40) & 0xff;
		data[NUM_BYTES - 7] = (value >> 48) & 0xff;
		data[NUM_BYTES - 8] = (value >> 56) & 0xff;
	}

	return BigIntegerDB<NUM_BYTES>(&data[0]);
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::GetMaximumValue()
{
	std::vector<uint8_t> data(NUM_BYTES);
	for (int i = 0; i < NUM_BYTES; i++)
	{
		data[i] = 0xFF;
	}

	return BigIntegerDB<NUM_BYTES>(&data[0]);
}

static uint8_t FromHexChar(char value)
{
	if (value <= '9' && value >= 0)
	{
		return value - '0';
	}

	if (value >= 'a')
	{
		return 10 + value - 'a';
	}

	return 10 + value - 'A';
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::FromHex(const std::string& hex)
{
	// TODO: Verify input size
	size_t index = 0;
	if (hex[0] == '0' && hex[1] == 'x')
	{
		index = 2;
	}

	std::string hexNoSpaces = "";
	for (size_t i = index; i < hex.length(); i++)
	{
		if (hex[i] != ' ')
		{
			hexNoSpaces += hex[i];
		}
	}

	std::vector<uint8_t> data(NUM_BYTES);
	for (size_t i = 0; i < hexNoSpaces.length(); i += 2)
	{
		data[i / 2] = (FromHexChar(hexNoSpaces[i]) * 16 + FromHexChar(hexNoSpaces[i + 1]));
	}

	return BigIntegerDB<NUM_BYTES>(&data[0]);
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator*(const BigIntegerDB<NUM_BYTES>& A) const
{
	const BigIntegerDB<NUM_BYTES> ZERO = BigIntegerDB<NUM_BYTES>::ValueOf(0);
	if (A == ZERO)
	{
		return ZERO;
	}

	BigIntegerDB<NUM_BYTES> multiplier = BigIntegerDB<NUM_BYTES>::ValueOf(1);
	BigIntegerDB<NUM_BYTES> nextMultiplier = BigIntegerDB<NUM_BYTES>::ValueOf(2);
	BigIntegerDB<NUM_BYTES> product = *this;

	while (nextMultiplier <= A)
	{
		multiplier = nextMultiplier;

		product += product;
		nextMultiplier += nextMultiplier;
	}

	BigIntegerDB<NUM_BYTES> remaining = A - multiplier;

	if (remaining > ZERO)
	{
		product = product + (*this * remaining);
	}

	return product;
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator+(const BigIntegerDB<NUM_BYTES>& addend) const
{
	std::vector<uint8_t> totalSum(NUM_BYTES);

	int carry = 0;

	for (int i = NUM_BYTES - 1; i >= 0; i--)
	{
		int digit1 = m_data[i];
		int digit2 = addend.GetData()[i];

		int sum = digit1 + digit2 + carry;

		if (sum > 255)
		{
			carry = 1;
			sum -= 256;
		}
		else
		{
			carry = 0;
		}

		totalSum[i] = sum;
	}

	return BigIntegerDB<NUM_BYTES>(&totalSum[0]);
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator-(const BigIntegerDB<NUM_BYTES>& amount) const
{
	std::vector<uint8_t> result(NUM_BYTES);

	int carry = 0;

	for (int i = NUM_BYTES - 1; i >= 0; i--)
	{
		int digit1 = m_data[i];
		int digit2 = amount.GetData()[i];

		int temp = digit1 - carry;
		carry = 0;

		if ((digit1 - carry) < digit2)
		{
			carry = 1;
			temp += 256;
		}

		result[i] = temp - digit2;
	}

	return BigIntegerDB<NUM_BYTES>(&result[0]);
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator+(const int addend) const
{
	BigIntegerDB biAddend = BigIntegerDB<NUM_BYTES>::ValueOf(addend);
	return *this + biAddend;
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator-(const int subtrahend) const
{
	BigIntegerDB biSubtrahend = BigIntegerDB<NUM_BYTES>::ValueOf(subtrahend);
	return *this - biSubtrahend;
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator*(const int multiplier) const
{
	BigIntegerDB biMultiplier = BigIntegerDB<NUM_BYTES>::ValueOf(multiplier);
	return *this * biMultipler;
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator/(const int divisor) const
{
	BigIntegerDB biDivisor = BigIntegerDB<NUM_BYTES>::ValueOf(divisor);
	return *this / biDivisor;
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator/(const BigIntegerDB<NUM_BYTES>& divisor) const
{
	BigIntegerDB<NUM_BYTES> remaining = *this;

	const BigIntegerDB<NUM_BYTES> ZERO = BigIntegerDB<NUM_BYTES>::ValueOf(0);

	BigIntegerDB<NUM_BYTES> multiplier = BigIntegerDB<NUM_BYTES>::ValueOf(0);
	BigIntegerDB<NUM_BYTES> prevTotal = divisor;
	BigIntegerDB<NUM_BYTES> total = divisor;

	while (total <= remaining)
	{
		prevTotal = total;
		total += total;

		if (multiplier == ZERO)
		{
			multiplier = BigIntegerDB<NUM_BYTES>::ValueOf(1);
		}
		else
		{
			multiplier += multiplier;
		}
	}

	total = prevTotal;

	BigIntegerDB<NUM_BYTES> quotient = multiplier;
	remaining = remaining - total;

	if (remaining >= divisor)
	{
		quotient = quotient + remaining / divisor;
	}

	return quotient;
}

template<size_t NUM_BYTES>
int BigIntegerDB<NUM_BYTES>::operator%(const int modulo) const
{
	BigIntegerDB<NUM_BYTES> quotient = *this / modulo;

	BigIntegerDB<NUM_BYTES> product = quotient * modulo;
	BigIntegerDB<NUM_BYTES> modResult = *this - product;

	return modResult.m_data[NUM_BYTES - 1];
}

template<size_t NUM_BYTES>
BigIntegerDB<NUM_BYTES> BigIntegerDB<NUM_BYTES>::operator%(const BigIntegerDB<NUM_BYTES>& modulo) const
{
	BigIntegerDB<NUM_BYTES> quotient = *this / modulo;

	BigIntegerDB<NUM_BYTES> product = quotient * modulo;
	BigIntegerDB<NUM_BYTES> modResult = *this - product;

	return modResult;
}