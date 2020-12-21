#include "BFOutput.h"

BFOutput::BFOutput()
{
	tbOutput = tvOutput.get_buffer();
	tvOutput.set_size_request(BFD_TEXTAREA_WIDTH, BFD_OUTPUT_HEIGHT);
	add(tvOutput);
}

BFOutput::~BFOutput()
{
}

error BFOutput::init()
{
	return 0;
}

void BFOutput::setText(const char *string)
{
	tbOutput->set_text(string);
}
