#include "Brainfuck.h"
#include "util/StringUtil.h"

//#define DEBUG_BRAINFUCK

Brainfuck::Brainfuck()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::Brainfuck()\n");
#endif
	input = 0;
	inputsize = 0;
	code = 0;
	codesize = 0;
	setMemoryModel(8);
	setCode("+++[-]");
	setInput(0, 0);
	reset();
}

Brainfuck::~Brainfuck()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::~Brainfuck()\n");
#endif
}

void Brainfuck::reset()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::reset()\n");
#endif
	memory.clear();
	memory.push_back(0);
	output.clear();
	output_changed = true;
	brackets.clear();

	emp = 0;
	eip = 0;
	einp = 0;
}

unsigned int Brainfuck::getEIP()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::getEIP()\n");
#endif
	return eip;
}

unsigned int Brainfuck::getEMP()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::getEMP()\n");
#endif
	return emp;
}

bfmem* Brainfuck::getMemory()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::getMemory()\n");
#endif
	return &memory;
}

bool Brainfuck::getOutputChanged()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::getOutputChanged()\n");
#endif
	bool b = output_changed;
	output_changed = false;
	return b;
}

char* Brainfuck::getOutput()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::getOutput()\n");
#endif
	const int len = output.size();
	char* out = new char[len+1];

	for (int i = 0; i < len; i++)
		out[i] = output[i];

	out[len] = 0;
	return out;
}

bool Brainfuck::getInputMissing()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::getInputMissing()\n");
#endif
	bool b = input_missing;
	input_missing = false;
	return b;
}

// --- Setter --- //

error Brainfuck::setCode(char* _code)
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::setCode()\n");
#endif
	reset();
	error err = validateCode(_code);
	if (err)
		return err;
	if (code != 0)
		delete code;
	if (0 == (code = StringUtil::copy(_code)))
		return "Out of memory.\n";
	codesize = StringUtil::length(code);
	return 0;
}

error Brainfuck::validateCode(char* code)
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::validateCode()\n");
#endif
	const int co = StringUtil::count(code, '[');
	const int cc = StringUtil::count(code, ']');

	if (co < cc)
		return "Missing opening bracket ([)";

	else if (cc < co)
		return "Missing closing bracket (])";

	return 0;
}

error Brainfuck::setMemoryModel(unsigned int bits)
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::setMemoryModel(%d)\n", bits);
#endif
	if (bits == 0 || bits > 32)
		return "Invalid Memory Model.\n";

	bits--;
	max_val = 1 << bits;
	max_val--;
	max_val += max_val;
	max_val++;
	return 0;
}

error Brainfuck::setInput(const char *i, const unsigned len)
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::setInput(%s)\n", i);
#endif

	if (input != 0)
		delete input;

	inputsize = len;
	einp = 0;

	if (i == 0) {
		input = 0;
		return 0;
	}

	input = StringUtil::copy(i, len);

	return input == 0 ? "Out of Memory." : 0;
}

error Brainfuck::execute(const char c)
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::execute(%c)\n", c);
#endif
	switch(c) {
		case '>': return empUp();
		case '<': return empDown();
		case ',': return memInput();
		case '.': return memOutput();
		case '+': return memIncrease();
		case '-': return memDecrease();
		case '[': return bracketOpen();
		case ']': return bracketClose();
		case 'B': return "Encountered Breakpoint.";
		default: return 0;
	}
}

error Brainfuck::step()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::step()\n");
#endif
	return eip == codesize ? "End of code." : execute(code[eip++]);
}

// --- Private Brainfuck --- //

error Brainfuck::empUp()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::step()\n");
#endif
	if (emp == memory.size() - 1) {
		memory.push_back(0);
		if (memory.size() - 1 == emp)
			return "Out of memory";
	}

	emp++;
	return 0;

}

error Brainfuck::empDown()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::step()\n");
#endif
	if (emp == 0)
		return "Memory underflow.";
	else emp--;

	return 0;
}

error Brainfuck::memInput()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::memInput()\n");
#endif
	if (einp >= inputsize) {
		eip--;
		input_missing = true;
		return "Missing user input";
	}

	memory[emp] = input[einp++];

	return 0;
}

error Brainfuck::memOutput()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::memOutput()\n");
#endif
	output.push_back((char)memory[emp]);

	output_changed = true;

	return 0;
}

error Brainfuck::memIncrease()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::memIncrease()\n");
#endif
	if (memory[emp] == max_val)
		memory[emp] = 0;
	else memory[emp]++;

	return 0;
}

error Brainfuck::memDecrease()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::memDecrease()\n");
#endif
	if (memory[emp] == 0) {
		memory[emp] = max_val;
	}
	else memory[emp]--;

	return 0;
}

error Brainfuck::bracketOpen()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::bracketOpen()\n");
#endif
	unsigned state = brackets.size();
	brackets.push_back(eip);

	if (memory[emp] > 0)
		return 0;

	char c;
	while (code[eip] != 0) {
		c = code[eip];

		if (c == ']')
			brackets.pop_back();
		else if (c == '[')
			brackets.push_back(eip);

		eip++;
		if (brackets.size() == state) {
			return 0;
		}
	}

	return "Missing Bracket.";
}

error Brainfuck::bracketClose()
{
#ifdef DEBUG_BRAINFUCK
	printf("Brainfuck::bracketClose()\n");
#endif
	if (brackets.size() == 0)
		return "Missing Bracket";

	int oldeip = brackets.back();

	if (memory[emp] == 0) {
		brackets.pop_back();
		return 0;
	}

	eip = oldeip;

	return 0;
}
