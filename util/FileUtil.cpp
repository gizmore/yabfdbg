#include "FileUtil.h"

#include "stdio.h"

long FileUtil::size(const char* path)
{
	FILE* file = fopen(path, "r");

	if (file == 0)
		return -1;

	fseek(file, 0, SEEK_END);
	long back = ftell(file);
	fclose(file);

	return back;
}

bool FileUtil::exists(const char* path)
{
	FILE* file = fopen(path, "r");

	if (file == 0)
		return false;

	fclose(file);

	return true;
}
