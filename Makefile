# YaBFDbg Makefile - all in one single makefile
# - By awe

BIN          = YaBFDbg
DIR  		 = yabfdbg
TARBALL      = $(DIR).tar.bz2

MAKE         = make
CC           = g++
CFLAGS       = -Wextra -O2 `pkg-config gtkmm-2.4 --cflags`
LDFLAGS      = `pkg-config gtkmm-2.4 --libs` 
BUILD        = build

UTILS_DIR    = util
DEPS_UTILS   = FileUtilGtk.h \
			   ArrayUtil.h \
			   FileUtil.h \
			   GizStr.h \
			   StringUtil.h

UTILS        = ArrayUtil.cpp \
			   FileUtil.cpp \
			   GizStr.cpp \
			   StringUtil.cpp \
			   FileUtilGtk.cpp

DEPS         = $(patsubst %,$(UTILS_DIR)/%,$(DEPS_UTILS)) \
			   BFInDialog.h \
			   BFMemTable.h \
			   Brainfuck.h \
			   BFDebugger.h \
			   defines.h \
			   BFInput.h \
			   BFOutput.h

SOURCES      = BFInDialog.cpp \
			   BFMemTable.cpp \
			   Brainfuck.cpp  \
			   BFDebugger.cpp \
			   BFInput.cpp \
			   BFOutput.cpp


SRC          = $(SOURCES) $(patsubst %,$(UTILS_DIR)/%,$(UTILS))
OBJS         = $(SRC:.cpp=.o)

.PHONY: all clean distclean dist

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f `find . -name "*.o"`
	rm -f `find . -name "*.swp"`
	rm -f `find . -name "*~"`
	rm -f `find . -name "*#"`
	rm -f `find . -name "core"`

distclean: clean
	rm -f $(BIN)

dist: distclean
	if [ ! -e ../$(DIR) ]; then mkdir ../$(DIR); cp -r . ../$(DIR);	$(MAKE) -C ../$(DIR) dist; else cd .. && tar -cvjf $(TARBALL) $(DIR) && cd -; fi
