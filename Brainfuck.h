#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include "vector"
#include "defines.h"

typedef std::vector<char>      bfout;
typedef std::vector<unsigned>  bfmem;
typedef std::vector<unsigned>  bfebp;

class Brainfuck
{
private:

	char  *code;
	char  *input;
	bfmem  memory;
	bfout  output;
	bfebp  brackets; // bracket pointer stack

	unsigned int eip;  // Instruction pointer
	unsigned int emp;  // Memory pointer
	unsigned int einp; // Input pointer

	unsigned int codesize;  // code length
	unsigned int inputsize; // Input length
	unsigned int max_val;   // memory type (8bit 16bit 32bit)

	bool output_changed;
	bool input_missing;

public: // --- Init --- //

	Brainfuck();
	~Brainfuck();
	void  reset();

public: // --- Getter --- //

	unsigned int getEIP();
	unsigned int getEMP();
	bfmem*       getMemory();
	bool         getOutputChanged();
	char*        getOutput();
	bool         getInputMissing();

public: // --- Setter --- //

	error setCode(char *code);
	error validateCode(char *code);
	error setMemoryModel(unsigned int bits);
	error setInput(const char *input, const unsigned length);

public: // --- Execute --- //

	error step();
	error execute(const char c);

private: // --- Private Brainfuck --- //

	error empUp();
	error empDown();
	error memInput();
	error memOutput();
	error memIncrease();
	error memDecrease();
	error bracketOpen();
	error bracketClose();

};

#endif // BRAINFUCK_H
