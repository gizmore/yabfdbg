#ifndef BFINPUT_H
#define BFINPUT_H

#include "gtkmm.h"
#include "defines.h"

class BFInput : public Gtk::ScrolledWindow
{
private:
	Gtk::TextView tvInput;
	Glib::RefPtr<Gtk::TextBuffer> tbInput;

public: // --- Init --- //

	BFInput();
	~BFInput();

	error init();
	void  reset();

public: // --- Getters --- //

	char*         getText();
	Gtk::TextIter getEndIter();
	Gtk::TextIter getStartIter();
	bool          isModified();

public: // --- Setters --- //

	void setText(const char *string);
	void setEnabled(bool b);
	void setModified(bool b);
	void setSelected(int char_num);

};



#endif // BFINPUT_H
