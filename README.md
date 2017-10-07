# BigIntegerDB - A fixed-width, header-only BigInteger class.
### Author: David Burkett (davidburkett38@gmail.com)
	
## Compilers Supported
	- Microsoft Visual C++ 2017
	- Mingw (Coming Soon)

## Dependencies
	- gtest
	
## Features
- Header-only
	- By being header-only, the feature can be easily included into other projects by just copying the file in. It only depends on the standard library.
- Fixed-width
	- Provides type-safety that is highly-beneficial for applications that encode or decode byte-arrays and integers of particular key-lengths.
	- Examples from cryptography:
		- An API for a SHA256 hash could return a BigIntegerDB<32> object, indicating to the consumer that the calculated hash will be exactly 32 bytes.
		- An API for a 128-bit ECDSA (Elliptic Curve Digital Signature Algorithm) could require a private-key parameter of BigIntegerDB<16>, so the consumer doesn't mistakenly pass in the public key (which would be 32 bytes).
- Fast Multiplication: Uses Double/Shift and add algorithm (https://en.wikipedia.org/wiki/Multiplication_algorithm#Shift_and_add)
- Fast Division and Modulo: TODO: Provide link to algorithm

## Supported Operations
- Addition
	- Adding 2 BigIntegers of the same byte-size together.
	- Adding a primitive int to a BigInteger.
- Subtraction
	- Subtracting 2 BigIntegers of the same byte-size together.
	- Subtracting a primitive int from a BigInteger.
- Multiplication
	- Multiplying 2 BigInteger of the same byte-size together.
	- Multiplying a BigInteger by a primitive int.
- Division
	- Dividing a BigInteger by another BigInteger of the same byte-size together.
	- Dividing a BigInteger by a primitive int.
- Modulus
	- Performing a modulo on 2 BigIntegers of the same byte-size together.
	- Performing a modulo on a BigInteger and a primitive int.
- Hex Conversion
	- Converts a hex string into a BigInteger.
- Unsigned char* conversion
	- Converts an unsigned char(uint8_t) array to a BigInteger.
	- Converts a BigInteger to an unsigned char(uint8_t) array.
- Primitive int conversion
	- Converts a primitive integer into a BigInteger.
	
## TODO
- Provide handling for overflows.
- Provide mixed byte-size operations.
- Handle hex inputs shorter than the NUM_BYTES parameter.
- Convert BigInteger to Hex String
- Provide exponent/power operations.
- Provide square root operation.