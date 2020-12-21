#include "ArrayUtil.h"

#include "stdio.h"
#include "string.h"

int* ArrayUtil::intArray(const unsigned size)
{
	int* back = new int[size];

	if (back) {
		for (unsigned i = 0; i < size; i++)
			back[i] = 0;
	}

	return back;
}

char* ArrayUtil::charArray(const unsigned size)
{
	char* back = new char[size];

	if (back) {
		for (unsigned i = 0; i < size; i++)
			back[i] = 0;
	}

	return back;
}

char* ArrayUtil::binFromHex(const char *hex_string, unsigned *result_len)
{
	const int len = strlen(hex_string);

	*result_len = len / 2;

	char *binary = new char[(*result_len)+1];

	if (binary == 0)
		return 0;

	int j = 0, i = 0;
	while (i < len) {

		char str[3] = { hex_string[i++], hex_string[i++], 0 };
		int val = 0;
		sscanf(str, "%2x", &val);
		binary[j++] = val;

	}

	binary[j] = 0;

	return binary;
}
