#include "BFDebugger.h"
#include "BFInDialog.h"
#include "util/ArrayUtil.h"
#include "util/StringUtil.h"
#include "util/GizStr.h"
#include "util/FileUtilGtk.h"

//#define DEBUG_BFDEBUGGER

// --- Main --- //

bool initThreading()
{
#ifdef DEBUG_BFDEBUGGER
	printf("initThreading()\n");
#endif
 	if (!Glib::thread_supported()) {
        Glib::thread_init();
        if (!Glib::thread_supported()) {
        	printf("Threading not supported!\n");
        	return false;
        }
    }

    return true;
}

int main(int argc, char** argv)
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::main()\n");
#endif

	if (!(initThreading()))
		return 1;

	Gtk::Main main_obj(argc, argv);

	BFDebugger bfd;


	const char *err = bfd.init();
	if (err)
		printf("Error: %s\n", err);
	else
		main_obj.run(bfd);

	return 0;
}

// --- Static --- //

BFDebugger* BFDebugger::instance = 0;

BFDebugger* BFDebugger::getInstance()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::getInstance()\n");
#endif
	return instance;
}

// --- Init --- //

BFDebugger::BFDebugger()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::BFDebugger()\n");
#endif
	filePath = 0;
	err = 0;
	input_changed = false;
	running = false;
	onTraceSpeedChanged(1);
}

BFDebugger::~BFDebugger()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::~BFDebugger()\n");
#endif
}

const char* BFDebugger::init()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::init()\n");
#endif
	const char* err = 0;

	set_title("YaBfDbg v1.0");

	instance = this;
	dispatcher.connect(sigc::mem_fun(this, &BFDebugger::dispatch));

	actionGroup = Gtk::ActionGroup::create();

	// --- File Menu --- //
	actionGroup->add(Gtk::Action::create("FileMenu", "File"));
	actionGroup->add(Gtk::Action::create(
		"FileLoad", "Load File", "Load a file."),
		*(new Gtk::AccelKey("<control>o", "<MainWindow>/FileMenu/FileLoad")),
		sigc::mem_fun(this, &BFDebugger::onLoadFile));
	actionGroup->add(Gtk::Action::create(
		"FileFormat", "Format Sourcecode", "Add return before and after brackets"),
		*(new Gtk::AccelKey("<control>f", "<MainWindow>/FileMenu/FileFormat")),
		sigc::mem_fun(this, &BFDebugger::onFormatSrc));
	actionGroup->add(Gtk::Action::create(
		"FileSave", "Save File", "Save current file."),
		*(new Gtk::AccelKey("<control>s", "<MainWindow>/FileMenu/FileSave")),
		sigc::mem_fun(this, &BFDebugger::onSaveFile));
	actionGroup->add(Gtk::Action::create(
		"FileSaveAs", "Save File as..", "Save current file under a different name."),
		*(new Gtk::AccelKey("<control><shift>s", "<MainWindow>/FileMenu/FileSaveAs")),
		sigc::mem_fun(this, &BFDebugger::onSaveFileAs));
	actionGroup->add(Gtk::Action::create(
		"FileQuit", "Quit", "Quit the program."),
		*(new Gtk::AccelKey("<control>q", "<MainWindow>/FileMenu/FileQuit")),
		sigc::mem_fun(this, &BFDebugger::onQuit));

	// --- Memory Menu --- //
	Gtk::RadioAction::Group grpMemory;
	actionGroup->add(Gtk::Action::create("MemoryMenu", "Memory"));
	actionGroup->add(Gtk::RadioAction::create(grpMemory, "Mem8", "8 bit memory"),
		*(new Gtk::AccelKey("<control>8", "<MainWindow>/MemoryMenu/Mem8")),
		sigc::bind(sigc::mem_fun(this, &BFDebugger::onMemoryModelChanged), 8 ));
	actionGroup->add(Gtk::RadioAction::create(grpMemory, "Mem16", "16 bit memory"),
		*(new Gtk::AccelKey("<control>6", "<MainWindow>/MemoryMenu/Mem16")),
		sigc::bind(sigc::mem_fun(this, &BFDebugger::onMemoryModelChanged), 16 ));
	actionGroup->add(Gtk::RadioAction::create(grpMemory, "Mem32", "32 bit memory"),
		*(new Gtk::AccelKey("<control>2", "<MainWindow>/MemoryMenu/Mem32")),
		sigc::bind(sigc::mem_fun(this, &BFDebugger::onMemoryModelChanged), 32 ));



	// --- Trace Menu --- //
	actionGroup->add(Gtk::Action::create("TraceMenu", "Trace"));
	Gtk::RadioAction::Group grpTrace;
	int trace_values[10] = {
		1, 2, 5,
		10, 20, 50,
		100, 200, 500, 1000 };
	char aname[32];
	char shown[32];
	for (int i = 0; i < 10; i++) {
		sprintf(aname, "Trace%d", trace_values[i]);
		sprintf(shown, "%d Hz", trace_values[i]);
		actionGroup->add(Gtk::RadioAction::create(grpTrace, aname, shown),
			sigc::bind(sigc::mem_fun(this, &BFDebugger::onTraceSpeedChanged), trace_values[i] ));
	}
	actionGroup->add(Gtk::Action::create(
		"TraceFaster", "Trace Faster", "Raises the trace speed"),
		*(new Gtk::AccelKey("<control>t", "<MainWindow>/TraceFaster")),
		sigc::mem_fun(this, &BFDebugger::onTraceFaster));
	actionGroup->add(Gtk::Action::create(
		"TraceSlower", "Trace Slower", "Lowers the trace speed"),
		*(new Gtk::AccelKey("<control><shift>t", "<MainWindow>/TraceFaster")),
		sigc::mem_fun(this, &BFDebugger::onTraceSlower));

	// --- Input Menu --- //
	actionGroup->add(Gtk::Action::create("InputMenu", "Input"));
	actionGroup->add(Gtk::Action::create(
		"SetInput", "Set Input", "Set user input."),
		*(new Gtk::AccelKey("<control>i", "<MainWindow>/InputMenu/SetInput")),
		sigc::mem_fun(this, &BFDebugger::onSetInput));

	// --- Sample Code Menu --- //
	actionGroup->add(Gtk::Action::create("SampleMenu", "Sample Code"));
	actionGroup->add(Gtk::Action::create(
		"SampleHello", "Hello World", "Set Hello-World sourcecode."),
		sigc::bind(sigc::mem_fun(this, &BFDebugger::onSetSampleCode), 0 ));

	// --- About Menu --- //
	actionGroup->add(Gtk::Action::create("AboutMenu", "About"));
	actionGroup->add(Gtk::Action::create(
		"ShowHelp", "Help", "Show help."),
		sigc::mem_fun(this, &BFDebugger::onHelp));
	actionGroup->add(Gtk::Action::create(
		"ShowAbout", "About", "Show about dialog."),
		sigc::mem_fun(this, &BFDebugger::onAboutDialog));

	// --- Toolbar --- //
	actionGroup->add(Gtk::Action::create(
		"Reset", "Reset", "Reset Brainfuck (F1)"),
		*(new Gtk::AccelKey("F1", "<MainWindow>/Reset")),
		sigc::mem_fun(this, &BFDebugger::onReset));
	actionGroup->add(Gtk::Action::create(
		"Run", "Run", "Run program (F2)"),
		*(new Gtk::AccelKey("F2", "<MainWindow>/Run")),
		sigc::mem_fun(this, &BFDebugger::onRun));
	actionGroup->add(Gtk::Action::create(
		"Trace", "Trace", "Trace program (F3)"),
		*(new Gtk::AccelKey("F3", "<MainWindow>/Trace")),
		sigc::mem_fun(this, &BFDebugger::onTrace));
	actionGroup->add(Gtk::Action::create(
		"Step", "Step", "Single step (F4)"),
		*(new Gtk::AccelKey("F4", "<MainWindow>/Step")),
		sigc::mem_fun(this, &BFDebugger::onStep));
	actionGroup->add(Gtk::Action::create(
		"Stop", "Stop", "Stop runner/tracer (F5)"),
		*(new Gtk::AccelKey("F5", "<MainWindow>/ToolBar/Stop")),
		sigc::mem_fun(this, &BFDebugger::onStop));


	// --- Create UI --- //
	uiManager = Gtk::UIManager::create();
	uiManager->insert_action_group(actionGroup);

	add_accel_group(uiManager->get_accel_group());

	Glib::ustring uiInfo =
		"<ui>"
		"  <accelerator action='TraceFaster' name='TraceFaster'/>"
		"  <accelerator action='TraceSlower' name='TraceSlower'/>"
		"  <accelerator action='Reset' name='Reset'/>"
		"  <accelerator action='Run' name='Run'/>"
		"  <accelerator action='Trace' name='Trace'/>"
		"  <accelerator action='Step' name='Step'/>"
		"  <accelerator action='Stop' name='Stop'/>"
		"  <menubar name='MenuBar'>"
		"    <menu action='FileMenu'>"
		"        <menuitem action='FileLoad'/>"
		"        <menuitem action='FileFormat'/>"
		"        <menuitem action='FileSave'/>"
		"        <menuitem action='FileSaveAs'/>"
		"        <menuitem action='FileQuit'/>"
		"    </menu>"
		"    <menu action='MemoryMenu'>"
		"        <menuitem action='Mem8'/>"
		"        <menuitem action='Mem16'/>"
		"        <menuitem action='Mem32'/>"
		"    </menu>"
		"    <menu action='TraceMenu'>"
		"        <menuitem action='Trace1'/>"
		"        <menuitem action='Trace2'/>"
		"        <menuitem action='Trace5'/>"
		"        <menuitem action='Trace10'/>"
		"        <menuitem action='Trace20'/>"
		"        <menuitem action='Trace50'/>"
		"        <menuitem action='Trace100'/>"
		"        <menuitem action='Trace200'/>"
		"        <menuitem action='Trace500'/>"
		"        <menuitem action='Trace1000'/>"
		"    </menu>"
		"    <menu action='InputMenu'>"
		"        <menuitem action='SetInput'/>"
		"    </menu>"
		"    <menu action='SampleMenu'>"
		"        <menuitem action='SampleHello'/>"
		"    </menu>"
		"    <menu action='AboutMenu'>"
		"        <menuitem action='ShowHelp'/>"
		"        <menuitem action='ShowAbout'/>"
		"    </menu>"
		"  </menubar>"
      "  <toolbar  name='ToolBar'>"
      "    <toolitem action='Reset'/>"
      "    <toolitem action='Run'/>"
      "    <toolitem action='Trace'/>"
      "    <toolitem action='Step'/>"
      "    <toolitem action='Stop'/>"
		"  </toolbar>"
		"</ui>";

	#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try {
		uiManager->add_ui_from_string(uiInfo);
	} catch(const Glib::Error& ex) {
		printf("building menus failed: %s\n", ex.what().c_str());
	}
	#else
	std::auto_ptr<Glib::Error> ex;
	uiManager->add_ui_from_string(uiInfo, ex);
	if(ex.get()) {
		printf("building menus failed: %s\n", ex.what().c_str());
	}
	#endif //GLIBMM_EXCEPTIONS_ENABLED

	if ((err = memTable.init()))
		return err;
	if ((err = input.init()))
		return err;
	if ((err = output.init()))
		return err;

	vboxText.pack_start(input, Gtk::PACK_SHRINK, BFD_SPACING);
	vboxText.pack_start(output, Gtk::PACK_SHRINK, BFD_SPACING);

	hbox.pack_start(vboxText, Gtk::PACK_SHRINK, BFD_SPACING);
	hbox.pack_start(memTable, Gtk::PACK_SHRINK, BFD_SPACING);

	vbox.pack_start(*(uiManager->get_widget("/MenuBar")), Gtk::PACK_SHRINK, BFD_SPACING);
	vbox.pack_start(*(uiManager->get_widget("/ToolBar")), Gtk::PACK_SHRINK, BFD_SPACING);
	vbox.pack_start(hbox, Gtk::PACK_SHRINK, BFD_SPACING);
	vbox.pack_start(statusbar, Gtk::PACK_SHRINK, BFD_SPACING);

	add(vbox);

	set_position(Gtk::WIN_POS_CENTER);

	show_all();

	onInputChanged();

	return err;
}

bool BFDebugger::initBrainfuck()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::initBrainfuck()\n");
#endif
	if (!input_changed)
		return true;

	if ((err = brainfuck.setCode(input.getText())))
		return false;

	input_changed = false;
	onReset();

	return true;
}

bool BFDebugger::on_delete_event(GdkEventAny *event)
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::on_delete_event()\n");
#endif
	printf("HERE\n");
	onQuit();
	return Gtk::Window::on_delete_event(event);
//	return true;
}

// --- Getters --- //

bfmem* BFDebugger::getMemory()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::getMemory()\n");
#endif
	return brainfuck.getMemory();
}

Brainfuck* BFDebugger::getBrainfuck()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::getBrainfuck()\n");
#endif
	return &brainfuck;
}

// --- TextArea --- //

void BFDebugger::onInputChanged()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onInputChanged()\n");
#endif
	input_changed = true;
}

void BFDebugger::onInputChangedB(const Gtk::TextIter& pos, const Glib::ustring& text, int bytes)
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onInputChangedB()\n");
#endif
	onInputChanged();
}

void BFDebugger::onInputChangedC(const Gtk::TextIter &start, const Gtk::TextIter &end)
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onInputChangedC()\n");
#endif
	onInputChanged();
}

// --- Toolbar --- //

void BFDebugger::onReset()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onReset()\n");
#endif
	running = false;
	statusbar.pop();
	brainfuck.reset();
	input.reset();
	dispatch();
}

void BFDebugger::onRun()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onRun()\n");
#endif
	if (running)
		return;

	statusbar.pop();

	if (!(initBrainfuck()))
		return;

	running = true;
	Glib::Thread::create(sigc::mem_fun(this, &BFDebugger::runner), false);
}

void BFDebugger::onTrace()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onTrace()\n");
#endif
	if (running)
		return;

	statusbar.pop();

	if (!(initBrainfuck()))
		return;

	running = true;
	Glib::Thread::create(sigc::mem_fun(this, &BFDebugger::tracer), false);
}

void BFDebugger::onStep()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onStep()\n");
#endif
	statusbar.pop();

	if (!(initBrainfuck()))
		return;

	err = brainfuck.step();

	dispatch();

}

void BFDebugger::onStop()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onStop()\n");
#endif
	running = false;
}

// --- File Menu --- //

void BFDebugger::onLoadFile()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onLoadFile()\n");
#endif
	running = false;

	const char *npath = FileUtilGtk::openFileDialog("Open a file", 0);

	if (npath == 0)
		return;

	if (!(err = loadFile(npath))) {

		if (filePath)
			delete filePath;
		filePath = npath;

	}
	dispatch();


}

error BFDebugger::loadFile(const char *path)
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::loadFile(%s)\n", path);
#endif

	if (!(FileUtilGtk::exists(path)))
		return "The file does not exist.";

	const unsigned size = FileUtilGtk::size(path);
	if (size == 0)
		return "The file seems not to be readable.";

	char *buffer = new char[size+1];

	if (!buffer)
		return "Out of memory.";

	FILE *file = fopen(path, "r");

	if (file) {
		unsigned bytesRead = fread(buffer, 1, size, file);
		printf("BR=%d|Size=%d\n", bytesRead, size);
		if (bytesRead > 0) {
			buffer[size] = 0;
			input.setText(buffer);
		}
		else err = "Error reading file.";
		fclose(file);
	}
	else err = "Cant open file.";

	delete buffer;

	return err;
}

void BFDebugger::onSaveFile()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onSaveFile()\n");
#endif
	running = false;

	if (!input_changed)
		return;

	if (filePath == 0)
		onSaveFileAs();
	else
		err = saveFile();

	dispatch();
}

void BFDebugger::onSaveFileAs()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onSaveFileAs()\n");
#endif
	running = false;

	const char *path = FileUtilGtk::saveFileDialog("Save File as..", 0);

	if (path != 0) {
		filePath = path;
		err = saveFile();
	}
	else
		err = "User canceled save file.";

	dispatch();
}

error BFDebugger::saveFile()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::saveFile()\n");
#endif
	if (filePath == 0)
		return "No file-path selected.";

	FILE *file = fopen(filePath, "w");

	if (!file)
		return "Cant open file for writing.";

	char *buffer = input.getText();
	const unsigned blen = StringUtil::length(buffer);

	if (buffer) {
		unsigned written = fwrite(buffer, 1, blen, file);
		fputc(0, file);
		if (written != blen)
			err = "Error writing file.";
		delete buffer;
	}
	else
		err = "Cant get input buffer.";

	fclose(file);
	return err;
}

void BFDebugger::onFormatSrc()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onFormatSrc()\n");
#endif
	running = false;

	char* code = input.getText();

	const int len = StringUtil::length(code);

	GizStr s(len+512);

	for (int i = 0; i < len; i++)	{
		if (code[i] == '[')
			s.append('\n');
		s.append(code[i]);
		if (code[i] == ']')
			s.append('\n');
	}

	input.setText(s.getBuffer());

}

void BFDebugger::onQuit()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onQuit()\n");
#endif
	onStop();
	hide();
}

// --- Memory Menu --- //

void BFDebugger::onMemoryModelChanged(unsigned int bits)
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onMemoryModelChanged(%d)\n", bits);
#endif
	err = brainfuck.setMemoryModel(bits);
	dispatch();
}

// --- Trace Menu --- //

void BFDebugger::onTraceSlower()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onTraceSlower()\n");
#endif
	if (m_usleep <= BFD_TRACE_MAX - BFD_TRACE_ALTER)
		m_usleep += BFD_TRACE_ALTER;
}

void BFDebugger::onTraceFaster()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onTraceFaster()\n");
#endif
	if (m_usleep >= BFD_TRACE_ALTER)
		m_usleep -= BFD_TRACE_ALTER;
}

void BFDebugger::onTraceSpeedChanged(unsigned int hertz)
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onTraceSpeedChanged(%d)\n", hertz);
#endif
	if (hertz == 0)
		return;
	m_usleep = 1000000 / hertz;
}

// --- Input Menu --- //

void BFDebugger::onSetInput()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onSetInput()\n");
#endif

	BFInDialog dlg;

	err = dlg.show();
	if (!err) {

		char *in = dlg.getInput();
		unsigned len = StringUtil::length(in);

		if (in) {
			if (dlg.isHex()) {
				char* t = ArrayUtil::binFromHex(in, &len);
				if (t) {
					delete in;
					in = t;
				}
			}
			if (!err)
				err = brainfuck.setInput(in, len);
			delete in;

		}
		else err = "Out of memory.";

	}

	dispatch();

}

// --- Sample Code --- //

void BFDebugger::onSetSampleCode(unsigned num)
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onSetSampleCode(%d)\n", num);
#endif
	static const unsigned n_samples = 1;
	static const char *samples[n_samples] = {
		"++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.",
	};

	if (num >= n_samples)
		return;

	input.setText(samples[num]);

}

// --- About Menu --- //

void BFDebugger::onHelp()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onHelp()\n");
#endif

	static const char *help =
		"This is a brainfuck interpreter and debugger.\n"
		"- You can step trace and run a brainfuck programm,\n"
		"  stop anytime, and continue from that position,\n"
		"  unless you modified the file.\n"
		"- You can set Breakpoints with the character 'B' (capital only)\n"
		"- If current user input is insufficient you will be prompted.\n"
		"-----------------------------------------------------------\n"
		"Brainfuck language:\n"
		"+ increase current memory address\n"
		"- decrease current memory address\n"
		"> shift memory address up\n"
		"< shift memory address down\n"
		"[] execute loop until current memroy address is zero\n"
		". output current memory address\n"
		", read one character from input into current memory address\n"
		"-----------------------------------------------------------\n"
		" --- Issues ---\n"
		"- The gui is using threads, but is not really thread safe :x,\n"
		"   so its not a bad idea to press stop before you make changes to the input\n"
		"- Sometimes the gui crashes for me (assert widget is visible fails)\n";

	input.setText(help);

}

void BFDebugger::onAboutDialog()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::onAboutDialog()\n");
#endif
	Gtk::MessageDialog dlg(*this, "About YaBfDbg", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, false);
	dlg.set_title("About YaBfDbg");
	dlg.set_secondary_text(
		"YaBfDbg v1.0\n"
		"Yet another Brainfuck Debugger\n"
		"Done in 2007 by Gizmore\n"
		"This software is private and unlicensed\n"
		"Use at your own risk\n"
		"You may reuse/redistribute source and binary in every form.\n"
		"You may even crack and hack this application.\n"
		"Contact me:\n"
		"AIM/ICQ: 276647844\n"
		"Email: guessmoor@yahoo.de\n"
	);
	dlg.run();
}

// --- Thread --- //

void BFDebugger::dispatch()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::dispatch()\n");
#endif

	input.setSelected(brainfuck.getEIP());
	memTable.onMemoryChanged();
	memTable.onScrollbarActivity();

	if (brainfuck.getOutputChanged())
		output.setText(brainfuck.getOutput());
	if (err) {
		statusbar.push(err);
		err = 0;
		if(brainfuck.getInputMissing()) {
			onSetInput();
//			BFInDialog bfin;
//			bfin.show();
		}

	}

}

void BFDebugger::tracer()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::tracer()\n");
#endif

	while (running) {
		Glib::usleep(m_usleep);
		if ((err = brainfuck.step()))
			break;
		dispatcher();
	}
	dispatcher();
	running = false;
}

void BFDebugger::runner()
{
#ifdef DEBUG_BFDEBUGGER
	printf("BFDebugger::runner()\n");
#endif
	while (running) {
		if ((err = brainfuck.step()))
			break;
	}
	dispatcher();
	running = false;
}

