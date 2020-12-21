#ifndef BFINDIALOG_H_INCLUDED
#define BFINDIALOG_H_INCLUDED

#include "gtkmm.h"
#include "defines.h"

class BFInDialog
{
private:
	Gtk::Dialog *dlg;
	Gtk::TextView tvInput;
	Glib::RefPtr<Gtk::TextBuffer> tbInput;

	Gtk::HBox hbox;
	Gtk::CheckButton cbHexInput;
	Gtk::Button btnOk;


public:

	BFInDialog();
	~BFInDialog();

	error show();

	bool  isHex();
	char* getInput();

	void onButtonOK();

};

#endif // BFINDIALOG_H_INCLUDED
