#ifndef DICTAC_GUI_PANEL_HASHPANEL_H__
#define DICTAC_GUI_PANEL_HASHPANEL_H__

#include "Brainfuck.h"
#include "defines.h"

#include "gtkmm.h"

class BFMemTable : public Gtk::VBox
{
public:

	BFMemTable();

	error init();

	void onScrollbarActivity(); // change texts

	void onMemoryChanged();

private:

	Gtk::Label lblHead;

	Gtk::HBox hbx;
	Gtk::Table table;
	Gtk::Entry tfMemory[BFD_MEMORY_ROWS*3];
	Gtk::VScrollbar scrollbar;
	Gtk::Adjustment adjustment;

	bfmem* memory;
	Brainfuck* brainfuck;
};

#endif // DICTAC_GUI_PANEL_HASHPANEL_H__
