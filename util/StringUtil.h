#ifndef GIZMORE_STRINGUTIL_H__
#define GIZMORE_STRINGUTIL_H__

typedef       char* newstr;   // New string that you will have to delete
typedef const char* conststr;

class StringUtil
{
public: // --- Construction --- //

	static newstr fromInt(int i);
	static newstr fromLong(long l);
	static newstr fromUnsigned(unsigned i);

	static newstr hexFromBinary(conststr binary, const int length);

	static newstr substr(conststr string, const int begin);
	static newstr substr(conststr string, const int begin, const int end);

	/* creates a new copy of the string from first occurance of splitter
	 * The Cpy function returns a new full copy instead of 0, when the splitter isnt found.
	*/
	static newstr substrFrom(conststr string, const char spliter);
	static newstr substrFrom(conststr string, conststr splitter);
	static newstr substrFromCpy(conststr string, const char spliter);
	static newstr substrFromCpy(conststr string, conststr splitter);

	/* creates a new copy of the string until first occurance of splitter
	 * The Cpy function returns a new full copy instead of 0, when the splitter isnt found.
	*/
	static newstr substrUntil(conststr string, const char spliter);
	static newstr substrUntil(conststr string, conststr splitter);
	static newstr substrUntilCpy(conststr string, const char spliter);
	static newstr substrUntilCpy(conststr string, conststr splitter);

	static newstr copy(conststr string); // slow
	static newstr copy(conststr string, const int len); // fast

	static newstr concat(conststr string1, conststr string2);

	static newstr trim(conststr string);
	static newstr trim(conststr string, const char trim);
	static newstr trim(conststr string, conststr trim);
	static newstr trimFront(conststr string, const char trim);
	static newstr trimFront(conststr string, conststr trim);

	/* see php documentation about ex/implode;
	 * The resulting array is terminated by a 0 pointer. */
	static newstr* explode(conststr string, const char exploder);
	static newstr  implode(conststr *array, const char imploder);

public:	// --- Conversion --- //

	static void toLower(char* string);
	static void toUpper(char* string);

public:	// --- Counting --- //

	static int count(conststr string, const char c);
	static int length(conststr string);

public:	// --- Indexes --- //

	/* will return -1 on failure; */
	static int  indexOf(conststr string, const char needle);
	static int  indexOf(conststr string, conststr needle);
	static int  indexOf(conststr string, const char needle, const int start_index);
	static int  indexOf(conststr string, conststr needle, const int start_index);

	static int  lastIndexOf(conststr string, const char needle);
	static int  lastIndexOf(conststr string, conststr needle);

	static bool startsWith(conststr string, const char needle);
	static bool startsWith(conststr string, conststr needle);

public: // --- Stuffz --- //

	static bool isHexString(conststr string);

};

#endif
