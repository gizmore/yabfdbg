#include "gtkmm.h"

#include "Brainfuck.h"
//#include "BFMenu.h"
//#include "BFToolbar.h"
#include "BFInput.h"
#include "BFOutput.h"
#include "BFMemTable.h"

class BFDebugger : public Gtk::Window
{
public: // --- Static --- //
	static BFDebugger* getInstance();

public: // --- Init --- //
	BFDebugger();
	~BFDebugger();
	const char* init();
	bool        initBrainfuck();

public: // --- Gtk::Window --- //

	bool on_delete_event(GdkEventAny *event);

public: // --- Getters --- //
	bfmem*     getMemory();
	Brainfuck* getBrainfuck();

public: // --- TextArea --- //
	void onInputChanged();
	void onInputChangedB(const Gtk::TextIter &pos, const Glib::ustring &text, int bytes);
	void onInputChangedC(const Gtk::TextIter &start, const Gtk::TextIter &end);

public: // --- Toolbar --- //
	void onReset();
	void onRun();
	void onTrace();
	void onStep();
	void onStop();

public: // --- File Menu --- //
	void  onLoadFile();
	error loadFile(const char *path);
	void  onSaveFile();
	void  onSaveFileAs();
	error saveFile();
	void  onFormatSrc();
	void  onQuit();

public: // --- Memory Menu --- //
	void onMemoryModelChanged(unsigned int bits);

public: // --- Trace Menu --- //
	void onTraceSlower();
	void onTraceFaster();
	void onTraceSpeedChanged(unsigned int hertz);

public: // --- Input Menu --- //
	void onSetInput();

public: // --- Sample Code --- //
	void onSetSampleCode(unsigned num);

public: // --- About Menu --- //
	void onHelp();
	void onAboutDialog();

public: // --- Thread --- //
	void dispatch();
	void tracer();
	void runner();

// ---------- Variables ---------- //
private: // --- Static --- //
	static BFDebugger* instance;

private: // --- Brainfuck --- //
	error     err;
	Brainfuck brainfuck;

private: // --- Thread --- //
	bool running;
	bool input_changed;
	int  m_usleep;
	Glib::Dispatcher dispatcher;

private: // --- Gtk Layout --- //

	Gtk::VBox vbox;
	Gtk::HBox hbox;
	Gtk::VBox vboxText;
	BFInput input;
	BFOutput output;
	BFMemTable memTable;
	Gtk::Statusbar statusbar;

	Glib::RefPtr<Gtk::UIManager> uiManager;
	Glib::RefPtr<Gtk::ActionGroup> actionGroup;

private: // --- File --- //

	const char *filePath;

};
