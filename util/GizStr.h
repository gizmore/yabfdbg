#ifndef GIZMORE_GIZSTR_H__
#define GIZMORE_GIZSTR_H__

class GizStr
{
private:

	char* buffer;
	unsigned int length;
	unsigned int buf_len;

public:

	GizStr(int buffer_size);
	GizStr(const char* string);
	GizStr(GizStr& dtstr);
	~GizStr();

	char* getBuffer();
	int   getLength();
	int   getBufferLength();

	bool   expand(unsigned int append_bytes);

	GizStr& append(const char c);
	GizStr& append(const char *string);
	GizStr& insert(const char *string, unsigned int pos);
	GizStr& operator+(const char *string);



};

#endif
