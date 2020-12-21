#include "BFInDialog.h"
#include "BFDebugger.h"
#include "util/StringUtil.h"

BFInDialog::BFInDialog()
{
	dlg = 0;
	tbInput = tvInput.get_buffer();
	cbHexInput.set_label("Hex Input");
	btnOk.set_label("OK");


}

BFInDialog::~BFInDialog()
{
}

error BFInDialog::show()
{
	dlg = new Gtk::Dialog("Enter User Input", true, true);

//	btnOk.signal_clicked().connect(sigc::mem_fun(this, &BFInDialog::onButtonOK));

	tvInput.set_size_request(BFD_INPUT_DLG_WIDTH, BFD_INPUT_DLG_HEIGHT);
//	dlg.set_size_request(BFD_INPUT_DLG_WIDTH, BFD_INPUT_DLG_HEIGHT);
	dlg->set_position(Gtk::WIN_POS_CENTER);

	dlg->get_vbox()->pack_start(tvInput, Gtk::PACK_SHRINK, BFD_SPACING);
	dlg->get_action_area()->pack_start(cbHexInput, Gtk::PACK_SHRINK, BFD_SPACING);
	dlg->add_button("OK", 1);
//	dlg->get_action_area()->pack_start(btnOk, Gtk::PACK_SHRINK, BFD_SPACING);

	dlg->show_all();

	int i = dlg->run();

	delete dlg;
	dlg = 0;

	return i == 1 ? 0 : "User aborted.";
}

bool BFInDialog::isHex()
{
	return cbHexInput.get_active();
}

char* BFInDialog::getInput()
{
	return StringUtil::copy(tbInput->get_text().c_str());
}

void BFInDialog::onButtonOK()
{
	dlg->hide();
}
