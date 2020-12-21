#include "BFMemTable.h"
#include "BFDebugger.h"
#include "util/StringUtil.h"

#include "vector"

//#define BFDBG_MEM_TABLE

BFMemTable::BFMemTable()
:
adjustment(
    0.0, // value
    0.0, 0.1, // lower, upper,
    1.0, BFD_MEMORY_ROWS, // step inc, page inc
    BFD_MEMORY_ROWS)      // page size
{
#ifdef BFDBG_MEM_TABLE
	printf("BFMemTable::MemTable()\n");
#endif
}

const char* BFMemTable::init()
{
#ifdef BFDBG_MEM_TABLE
	printf("BFMemTable::init()\n");
#endif
	brainfuck = BFDebugger::getInstance()->getBrainfuck();
	memory = brainfuck->getMemory();

	lblHead.set_text("Memory");
	scrollbar.signal_value_changed().connect(sigc::mem_fun(*this, &BFMemTable::onScrollbarActivity));

	int j = 0;
	for (int y = 0; y < BFD_MEMORY_ROWS; y++)
		for (int x = 0; x < 3; x++)
			table.attach(tfMemory[j++], x, x+1, y, y+1, Gtk::FILL, Gtk::EXPAND, 0, 0);

	j = 0;
	for (int y = 0; y < BFD_MEMORY_ROWS; y++){
		tfMemory[j++].set_width_chars(6);
		tfMemory[j++].set_width_chars(10);
		tfMemory[j++].set_width_chars(3);
	}

	onMemoryChanged();
	onScrollbarActivity();

	hbx.pack_start(table, Gtk::PACK_SHRINK, BFD_SPACING);
	hbx.pack_start(scrollbar, Gtk::PACK_SHRINK, BFD_SPACING);

	pack_start(lblHead, Gtk::PACK_SHRINK, BFD_SPACING);
	pack_start(hbx, Gtk::PACK_SHRINK, BFD_SPACING);

	return 0;

}

void BFMemTable::onScrollbarActivity()
{
#ifdef BFDBG_MEM_TABLE
	printf("BFMemTable::onScrollbarActivity()\n");
#endif

//	vector<int>::iterator it = memory->begin();
	const int emp = brainfuck->getEMP();
	bfmem* memory = brainfuck->getMemory();
	const int len = memory->size();
	const int start = (int)round(scrollbar.get_value());
//	int taken = 0;
	int t = 0;

	for (int i = start; i < start+BFD_MEMORY_ROWS; i++) {

		char* num = 0;

		{
			char c[64]; // Buffer overflow hack here; nice decleration, isnt it =?
			sprintf(c, "%s%u", i == emp ? "=>" : "", (i));
			tfMemory[t].set_text(c);
		}
		t++;

		if (i >= len) {
			tfMemory[t++].set_text("---");
			tfMemory[t++].set_text("-");
			continue;
		}


		int n = (*memory)[i];
		num = StringUtil::fromUnsigned(n);
		char ascii[3] = { n > 127 ? ' ' : n, 0, 0 };
		if (num) {
			tfMemory[t++].set_text(num);
			tfMemory[t++].set_text(ascii);
			delete num;
		}
		else t += 2;
	}
}

void BFMemTable::onMemoryChanged()
{
#ifdef BFDBG_MEM_TABLE
	printf("BFMemTable::onMemoryChanged()\n");
#endif
	const int len = memory->size();

	const int emp = brainfuck->getEMP();

	double val = adjustment.get_value();

	if (val > emp || val+BFD_MEMORY_ROWS <= emp)
		adjustment.set_value(emp);

	adjustment.set_upper(len);
	scrollbar.set_range(0, len == 0 ? 0.01 : len);
	scrollbar.set_adjustment(adjustment);

}
