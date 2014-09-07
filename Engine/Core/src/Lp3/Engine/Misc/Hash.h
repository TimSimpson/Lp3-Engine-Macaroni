#ifndef MISC_HASH_H
#define MISC_HASH_H

#include "../../LE.h"

BEGIN_NAMESPACE(Lp3, Engine, Misc)

void * GenerateVoidHash(const char * string);

void * GenerateVoidHash(const char * string, size_t len);

template<typename t>
t GenerateHash(const char * string)
{
	return reinterpret_cast<t>(GenerateVoidHash(string));
}

template<typename t>
t GenerateHash(const char * string, size_t len)
{
	return reinterpret_cast<t>(GenerateVoidHash(string, len));
}

END_NAMESPACE

#endif