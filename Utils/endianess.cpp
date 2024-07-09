#include "Utils/endianess.h"

const unsigned char Endianess::BIG_ENDIAN_CAMARON = 0;
const unsigned char Endianess::LITTLE_ENDIAN_CAMARON = 1;

Endianess::Endianess()
{
}
bool Endianess::isBigEndian(){
    // See here: https://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
	union {
		int i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1;
}

unsigned char Endianess::findEndianness(){
	return (Endianess::isBigEndian())?Endianess::BIG_ENDIAN_CAMARON:Endianess::LITTLE_ENDIAN_CAMARON;
}
