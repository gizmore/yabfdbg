#include "StringUtil.h"

#include "ctype.h"
#include "stdio.h"
#include "string.h"

// Debug
#ifdef DICTAC_DEBUG_VERBOSE
#  define DTDBG_UTIL_STRINGUTIL
#endif

// --- Construction --- //

newstr  StringUtil::fromInt(int i)
{
	char* back = new char[12];

	sprintf(back, "%d", i);

	return back;
}

newstr  StringUtil::fromLong(long l)
{
	char* back = new char[24];

	sprintf(back, "%.0f", (double)l);

	return back;
}

newstr  StringUtil::fromUnsigned(unsigned i)
{
	char* back = new char[12];

	sprintf(back, "%u", i);

	return back;
}

newstr  StringUtil::hexFromBinary(const char* bin, const int len)
{
	char* hex = new char[len*2+1];

	int j = 0;
	for (int i = 0; i < len; i++) {
		sprintf(hex+j, "%02x", (unsigned char)bin[i]);
		j += 2;
	}
	hex[j] = 0;

	return hex;
}

newstr  StringUtil::substr(const char* s, const int begin)
{
	return substr(s, begin, length(s));
}

newstr  StringUtil::substr(const char* s, const int begin, const int end)
{
    const int len = end - begin;

    char* back = new char[len+1];

    int j = 0;
    for (int i = begin; i < end; i++)
        back[j++] = s[i];

    back[j] = 0;

    return back;
}

newstr StringUtil::substrFrom(const char* s, const char c)
{
	const int i = indexOf(s, c);

	return i == -1 ? 0 : substr(s, i+1);
}

newstr StringUtil::substrFrom(const char* s, const char* sp)
{
	const int len = strlen(sp);
	const int i = indexOf(s, sp);

	return i == -1 ? 0 : substr(s, i+len);
}

newstr StringUtil::substrFromCpy(const char* s, const char c)
{
	newstr back = substrFrom(s, c);

	return back == 0 ? copy(s) : back;
}

newstr StringUtil::substrFromCpy(const char* s, const char* sp)
{
	newstr back = substrFrom(s, sp);

	return back == 0 ? copy(s) : back;
}

newstr StringUtil::substrUntil(const char* s, const char c)
{
	const int i = indexOf(s, c);

	return i < 0 ? 0 : substr(s, 0, i);
}

newstr StringUtil::substrUntil(const char* s, const char* sp)
{
	const int i = indexOf(s, sp);

	return i < 0 ? 0 : substr(s, 0, i);
}

newstr StringUtil::substrUntilCpy(const char* s, const char c)
{
	newstr back = substrUntil(s, c);

	return back == 0 ? copy(s) : back;
}

newstr StringUtil::substrUntilCpy(const char* s, const char* sp)
{
	newstr back = substrUntil(s, sp);

	return back == 0 ? copy(s) : back;
}

newstr  StringUtil::copy(const char* s)
{
	return copy(s, length(s));
}

newstr  StringUtil::copy(const char* s, const int len)
{
	char* back = new char[len+1];

	if (back != 0) {
		memcpy(back, s, len);
		back[len] = 0;
	}

	return back;
}

newstr  StringUtil::concat(const char* a, const char* b)
{
	int la = length(a);
	int lb = length(b);
	char* back = new char[la+lb+1];

	memcpy(back, a, la);
	memcpy(back+la, b, lb);
	back[la+lb] = 0;

	return back;
}

newstr StringUtil::trim(const char* s)
{
	return trim(s, " \n\t");
}

newstr StringUtil::trim(const char* s, const char trim)
{
	const int len = length(s);
	int start = 0;
	int end = 0;

	for (start = 0; start < len; start++) {
		if (s[start] != trim)
			break;
	}

	for (end = len-1; end > start; end--) {
		if (s[end] != trim)
			break;
	}

	int nlen = end - start;

	return copy(s+start, nlen+1);
}

newstr StringUtil::trim(const char* s, const char* trim)
{
	const int len = length(s);
	const int tlen = length(trim);
	int start = 0;
	int end = 0;
	int i;

	for (start = 0; start < len; start++) {
		for (i = 0; i < tlen; i++) {
			if (s[start] == trim[i]) {
				break;
			}
		}
		if (i == tlen)
			break;
	}

	for (end = len-1; end > start; end--) {
		for (i = 0; i < tlen; i++) {
			if (s[end] == trim[i]) {
				break;
			}
		}
		if (i == tlen)
			break;
	}

	int nlen = end - start;

	return copy(s+start, nlen+1);
}

newstr StringUtil::trimFront(const char* s, const char trim)
{
	const int len = length(s);
	int start = 0;

	for (start = 0; start < len; start++) {
		if (s[start] != trim)
			break;
	}

	int nlen = len - start;

	return copy(s+start, nlen+1);
}

newstr StringUtil::trimFront(const char* s, const char* trim)
{
	const int len = length(s);
	const int tlen = length(trim);
	int start = 0;
	int i;

	for (start = 0; start < len; start++) {
		for (i = 0; i < tlen; i++) {
			if (s[start] == trim[i]) {
				break;
			}
		}
		if (i == tlen)
			break;
	}

	int nlen = len - start;

	return copy(s+start, nlen+1);
}

newstr* StringUtil::explode(const char* s, const char ex)
{
    const int cex = count(s, ex); // count occurance of exploder

    newstr* result = new newstr[cex+2]; // reserve mem for at least cex + 1 + null

    int i = 0; // current char
    int start = 0; // start of current string
    int taken = 0; // strings taken

    while (true) { // for a while...

        if (s[i] == ex || s[i] == 0) { // Exploder or 0 char ?

			// copy current string from start to exploder
            newstr temp = substr(s, start, i);
            if (length(temp) > 0) { // took anything ?
                result[taken++] = temp;
            }

			// reset start for next string
            start = i+1;

			// 0 char quits
            if (s[i] == 0) {
                result[taken] = 0;
                return result;
            }

        } // is exploder or 0 ?
        i++;
    } // For a while
}

newstr  StringUtil::implode(const char** array, const char imploder)
{
	char* back = new char[64];
	sprintf(back, "implode wont work yet :(");
	return back;
}

// --- Conversion --- //

void StringUtil::toLower(char* s)
{
	int i = 0;

	while (s[i] != 0) {

		s[i] = tolower(s[i]);
		i++;

	}
}

void StringUtil::toUpper(char* s)
{
	int i = 0;

	while (s[i] != 0) {

		s[i] = toupper(s[i]);
		i++;

	}
}

// --- Counting --- //

int StringUtil::count(const char* s, const char c)
{
    int result = 0;
    int i = 0;

    while(s[i] != 0) {
        if (s[i++] == c)
            result++;
    }

    return result;

}

int StringUtil::length(const char* s)
{
	return strlen(s);
}

// --- Indexes --- //

int StringUtil::indexOf(const char* s, const char needle)
{
	const char* t = strchr(s, needle);

	return t == 0 ? -1 : t - s;
}

int StringUtil::indexOf(const char* s, const char* needle)
{
	const char *t = strstr(s, needle);

	return t == 0 ? -1 : t - s;
}

int StringUtil::indexOf(const char* s, const char needle, const int si)
{
	return indexOf(s+si, needle);
}

int StringUtil::indexOf(const char* s, const char* needle, const int si)
{
	return indexOf(s+si, needle);
}

int StringUtil::lastIndexOf(const char* s, const char needle)
{
	const char* t = strrchr(s, needle);

	return t == 0 ? -1 : t - s;
}

int StringUtil::lastIndexOf(const char* s, const char* needle)
{
	const char* t = s;
	int index = -1;

	while (0 != (t = strstr(t, needle)))
		index = t - s;

	return index;
}

// --- startsWith --- //

bool StringUtil::startsWith(const char* s, const char needle)
{
	return indexOf(s, needle) == 0;
}

bool StringUtil::startsWith(const char* s, const char* needle)
{
	return indexOf(s, needle) == 0;
}

bool StringUtil::isHexString(const char* s)
{
	static const char* hex = "1234567890abcdefABCDEF";
	static const int hl = strlen(hex);

	const int len = length(s);

	int j = 0;

	for (int i = 0; i < len; i++) {
		for (j = 0; j < hl; j++) {
			if (s[i] == hex[j]) {
				break;
			}
		}
		if (j == hl)
			return false;
	}
	return true;
}
