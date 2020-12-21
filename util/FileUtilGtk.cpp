#include "FileUtilGtk.h"
#include "StringUtil.h"

#include "gtkmm/filechooserdialog.h"

#ifdef DICTAC_DEBUG_VERBOSE
#  define DICTAC_DBG_GUI_FILEUTIL
#endif

const char* FileUtilGtk::openFileDialog(const char* title, const char* folder)
{
#ifdef DICTAC_DBG_GUI_FILEUTIL
    printf("FileUtilGtk::openFileDialog('%s') ... \n", title);
#endif

	Gtk::FileChooserDialog fcd(title, Gtk::FILE_CHOOSER_ACTION_OPEN, "blub");

	fcd.set_select_multiple(false);
	fcd.set_show_hidden(false);
	fcd.add_button("add", 5);

	if (folder != 0)
		fcd.set_current_folder(folder);

	int result = fcd.run();

	if (result != 5)
        return 0;

	const char* filename = fcd.get_filename().c_str();

#ifdef DICTAC_DBG_GUI_FILEUTIL
	printf("FileUtil::openFileDialog() result = '%s'\n", filename);
#endif

	return StringUtil::copy(filename);

}

const char* FileUtilGtk::saveFileDialog(const char *title, const char *folder)
{
#ifdef DICTAC_DBG_GUI_FILEUTIL
    printf("FileUtilGtk::saveFileDialog('%s') ... \n", title);
#endif
	Gtk::FileChooserDialog fcd(title, Gtk::FILE_CHOOSER_ACTION_SAVE, "blub");

	fcd.set_select_multiple(false);
	fcd.set_show_hidden(false);
	fcd.add_button("Save", 5);

	if (folder != 0)
		fcd.set_current_folder(folder);

	int result = fcd.run();

	if (result != 5)
        return 0;

	const char* filename = fcd.get_filename().c_str();

#ifdef DICTAC_DBG_GUI_FILEUTIL
	printf("FileUtil::saveFileDialog() result = '%s'\n", filename);
#endif

	return StringUtil::copy(filename);
}
