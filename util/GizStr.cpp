#include "GizStr.h"

#include "string.h"
#include "stdio.h"

GizStr::GizStr(int size)
{
	buffer = new char[size+1];
	length = 0;
	buffer[length] = 0;
	buf_len = size;
}

GizStr::GizStr(const char *s)
{
	length = strlen(s);
	buf_len = length + 1;
	buffer = new char[buf_len];
	memcpy(buffer, s, buf_len);
}

GizStr::GizStr(GizStr& dts)
{
	GizStr(dts.buffer);
}

GizStr::~GizStr()
{
}

int GizStr::getLength()
{
	return length;
}

char* GizStr::getBuffer()
{
	return buffer;
}

int GizStr::getBufferLength()
{
	return buf_len;
}

bool GizStr::expand(unsigned int n)
{
	unsigned int assured = buf_len - length;

	if (assured >= n)
		return true;

	char* old = buffer;

	int nlen = buf_len + n;
	buffer = new char[nlen];
	if (buffer == 0) {
		printf("DTStr::expand(size=%d) - OUT OF MEMORY!\n", n);
		buffer = old;
		return false;
	}
	buf_len = nlen;

	memcpy(buffer, old, length+1);

	delete old;

	return true;
}

GizStr& GizStr::append(const char c)
{
	if(expand(1)) {
		buffer[length] = c;
		length++;
		buffer[length] = 0;
	}

	return *this;
}

GizStr& GizStr::append(const char *s)
{
	int sl = strlen(s);

	if(expand(sl)) {
		memcpy(buffer+length, s, sl);
		length += sl;
		buffer[length] = 0;
	}

	return *this;
}

GizStr& GizStr::insert(const char *s, unsigned int pos)
{
	return *this;
}

GizStr& GizStr::operator + (const char* s)
{
	return append(s);
}
