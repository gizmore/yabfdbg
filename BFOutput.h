#ifndef BFOUTPUT_H_INCLUDED
#define BFOUTPUT_H_INCLUDED

#include "gtkmm.h"
#include "defines.h"

class BFOutput : public Gtk::ScrolledWindow
{
private:
	Gtk::TextView tvOutput;
	Glib::RefPtr<Gtk::TextBuffer> tbOutput;

public:

	BFOutput();
	~BFOutput();

	error init();

	void setText(const char *string);

};
#endif // BFOUTPUT_H_INCLUDED
