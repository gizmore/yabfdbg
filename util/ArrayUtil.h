#ifndef GIZMORE_ARRAYUTIL_H__
#define GIZMORE_ARRAYUTIL_H__

class ArrayUtil
{
public:

	/* create an array of size 'size' and fill it with 0's*/
	static int*  intArray(const unsigned size);
	static char* charArray(const unsigned size);

	static char* binFromHex(const char *hex_string, unsigned *result_len);

};

#endif
