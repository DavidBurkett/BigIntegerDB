#pragma once

/*
* This code is free for all purposes without any express guarantee it works.
*
* Author: David Burkett (davidburkett38@gmail.com)
*/

#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

//
// BigIntegerDB is a fixed-width BigInteger class that allows type-safety when needed.
// This is especially useful for cryptography, where inputs and outputs are of an expected size.
// For example, a SHA256 method would return a BigIntegerDB<32> object, notifying the consumer that the output is 32 bytes (256 bits).
//
template<size_t NUM_BYTES>
class BigIntegerDB
{
private:
	std::vector<unsigned char> m_vector;

public:
	BigIntegerDB() = default;

	// TODO: If class is ever made variable width, this would either need to take in a vector, or take in a length parameter
	BigIntegerDB(const unsigned char* data)
		: m_vector(data, data + NUM_BYTES)
	{
	}

	~BigIntegerDB() = default;

	static BigIntegerDB valueOf(const unsigned char value)
	{
		std::vector<unsigned char> data(NUM_BYTES);
		data[(NUM_BYTES) - 1] = value;
		return BigIntegerDB(&data[0]);
	}

	static BigIntegerDB FromHex(const std::string& hex)
	{
		// TODO: Verify input size
		int index = 0;
		if (hex[0] == '0' && hex[1] == 'x')
		{
			index = 2;
		}

		std::vector<unsigned char> data(NUM_BYTES);
		for (int i = index; i < hex.length(); i += 2)
		{
			data[(i - index) / 2] = (FromHexChar(hex[i]) * 16 + FromHexChar(hex[i + 1]));
		}

		return BigIntegerDB(&data[0]);
	}

	const std::vector<unsigned char>& GetVector() const
	{
		return m_vector;
	}

	std::vector<unsigned char> GetLittleEndianVector() const
	{
		std::vector<unsigned char> littleEndian(m_vector.size());
		std::reverse_copy(m_vector.cbegin(), m_vector.cend(), littleEndian.begin());
		return littleEndian;
	}

	BigIntegerDB BigIntegerDB::operator*(const BigIntegerDB& A)
	{
		std::vector<unsigned char> temp;

		int result = 0;
		for (int i = 0; i < A.numBytes; i++)
		{
			int carry = 0;
			int k = i;

			for (int j = 0; j < this->numBytes; j++)
			{
				result = A.m_vector[i] * this->m_vector[j] + temp[k];
				temp[k] = (result + carry) % 256;
				carry = (result + carry) / 256;
				k++;
			}

			if (carry != 0)
			{
				temp[k] = temp[k] + carry;
				k++;
			}
		}

		BigIntegerDB final(temp, k);
		return final;
	}

	BigIntegerDB operator+(const BigIntegerDB& addend)
	{
		std::vector<unsigned char> totalSum(NUM_BYTES);

		int carry = 0;

		for (int i = NUM_BYTES - 1; i >= 0; i--)
		{
			int digit1 = m_vector[i];
			int digit2 = addend.GetVector()[i];

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

	BigIntegerDB operator-(const BigIntegerDB& amount)
	{
		std::vector<unsigned char> result(NUM_BYTES);

		int carry = 0;

		for (int i = NUM_BYTES - 1; i >= 0; i--)
		{
			int digit1 = m_vector[i];
			int digit2 = amount.GetVector()[i];

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

	BigIntegerDB operator*(const int multiplier)
	{
		BigIntegerDB temp(&m_vector[0]);
		for (int i = 1; i < multiplier; i++)
		{
			temp = temp + *this;
		}

		return temp;
	}

	BigIntegerDB operator/(const int divisor)
	{
		std::vector<unsigned char> quotient(NUM_BYTES);

		int remainder = 0;
		for (int i = 0; i < NUM_BYTES; i++)
		{
			remainder = remainder * 256 + m_vector[i];
			quotient[i] = remainder / divisor;
			remainder -= quotient[i] * divisor;
		}

		return BigIntegerDB(&quotient[0]);
	}

	int operator%(const int modulo)
	{
		BigIntegerDB quotient = *this / modulo;

		BigIntegerDB product = quotient * modulo;
		BigIntegerDB modResult = *this - product;

		return modResult.m_vector[NUM_BYTES - 1];
	}

	inline bool operator<(const BigIntegerDB& rhs) const
	{
		for (int i = 0; i < NUM_BYTES; i++)
		{
			if (this->m_vector[i] < rhs.m_vector[i])
			{
				return true;
			}
			else if (this->m_vector[i] > rhs.m_vector[i])
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
		for (int i = 0; i < NUM_BYTES; i++)
		{
			if (this->m_vector[i] != rhs.m_vector[i])
			{
				return false;
			}
		}

		return true;
	}

	inline bool operator<=(const BigIntegerDB& rhs) const
	{
		return *this < rhs || *this == rhs;
	}

	inline bool operator>=(const BigIntegerDB& rhs) const
	{
		return *this > rhs || *this == rhs;
	}

private:
	static unsigned char FromHexChar(char value)
	{
		if (value <= '9')
		{
			return value - '0';
		}

		return 10 + value - 'a';
	}
};