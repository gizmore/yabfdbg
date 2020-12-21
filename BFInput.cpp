#include "BFInput.h"
#include "BFDebugger.h"
#include "util/StringUtil.h"

BFInput::BFInput()
{
	tbInput = tvInput.get_buffer();
	tvInput.set_size_request(BFD_TEXTAREA_WIDTH, BFD_CODE_HEIGHT);
	add(tvInput);
}

BFInput::~BFInput()
{
	//dtor
}

error BFInput::init()
{
	BFDebugger* bfd = BFDebugger::getInstance();
	tbInput->signal_erase().connect(sigc::mem_fun(bfd, &BFDebugger::onInputChangedC));
	tbInput->signal_insert().connect(sigc::mem_fun(bfd, &BFDebugger::onInputChangedB));
//	tvInput.signal_insert_at_cursor().connect(sigc::mem_fun(bfd, &BFDebugger::onInputChanged));
//	tbInput->signal_modified_changed().connect(sigc::mem_fun(bfd, &BFDebugger::onInputChangedB));
//	tbInput->signal_changed().connect(sigc::mem_fun(bfd, &BFDebugger::onInputChangedB));
	return 0;
}

void BFInput::reset()
{
	setSelected(0);
}


// --- Getters --- //

char* BFInput::getText()
{
	return StringUtil::copy(tbInput->get_text().c_str());
}

Gtk::TextIter BFInput::getEndIter()
{
	return tbInput->end();
}

Gtk::TextIter BFInput::getStartIter()
{
	return tbInput->begin();
}

bool  BFInput::isModified()
{
		return tbInput->get_modified();
}

// --- Setters --- //

void BFInput::setText(const char *string)
{
	tbInput->set_text(string);
}

void BFInput::setEnabled(bool b)
{
	tvInput.set_editable(b);
}

void BFInput::setModified(bool b)
{
	tbInput->set_modified(b);
}

void BFInput::setSelected(int char_num)
{
	Gtk::TextIter it = tbInput->get_iter_at_offset(char_num);
	Gtk::TextIter ite = tbInput->get_iter_at_offset(char_num+1);
	tbInput->select_range(it, ite);
}

