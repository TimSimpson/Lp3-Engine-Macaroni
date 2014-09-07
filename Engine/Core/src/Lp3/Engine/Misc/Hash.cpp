#ifndef MISC_HASH_CPP
#define MISC_HASH_CPP

#include "Hash.h"
#include <string>
#include <string.h>

BEGIN_NAMESPACE(Lp3, Engine, Misc)

/** This code is from Game Coding Complete, which said it based it off of
 * something in Zlib. pg 345 */
void * GenerateVoidHash(const char * string, size_t stringLength)
{
	// largest prime smaller than 65536
	unsigned long BASE = 65521L;

	// NMAX is the largest n such that
	// 255n(n + 1)/2 + (n + 1)(BASE - 1) <= 2^32-1
	unsigned long NMAX = 5552;

	#define DO1(buf, i) {s1 += tolower(buf[i]); s2 += s1; }
	#define DO2(buf, i) DO1(buf, i); DO1(buf, i + 1);
	#define DO4(buf, i) DO2(buf, i); DO2(buf, i + 2);
	#define DO8(buf, i) DO4(buf, i); DO4(buf, i + 4);
	#define DO16(buf)   DO8(buf, 0); DO8(buf, 8);

	if (string == nullptr)
		return nullptr;

	// Leave out wild card code from Game Coding Complete.

	unsigned long s1 = 0;
	unsigned long s2 = 0;

	for (size_t len = stringLength; len>0; )
	{
		unsigned long k = len < NMAX ? len : NMAX;

		len -= k;

		// Have no idea if the identation below was intentional in the book...
		while(k >= 16)
		{
			DO16(string);
				string += 16;
			k-= 16;
		}

		if (k != 0)
		{
			do
			{
				s1 += *string ++;
				s2 += s1;
			} while( --k );
		}

		s1 %= BASE;
		s2 %= BASE;
	} // end of for loop? Indentation in book was odd.

	#pragma warning(push)
	#pragma warning(disable : 4312)

	return reinterpret_cast<void *>( (s2 << 16) | s1 );

	#pragma warning(pop)
	#undef D01
	#undef DO2
	#undef DO4
	#undef DO8
	#undef DO16
}

void * GenerateVoidHash(const char * string)
{
	size_t stringLength = strlen(string);
	return GenerateVoidHash(string, stringLength);
}

END_NAMESPACE

#endif
