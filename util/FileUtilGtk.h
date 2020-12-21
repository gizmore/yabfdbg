#ifndef GIZMORE_FILEUTILGTK_H__
#define GIZMORE_FILEUTILGTK_H__

#include "FileUtil.h"

class FileUtilGtk : public FileUtil
{
public:

	// returns a new string (delete it), that contains the selected file
	static const char* openFileDialog(const char *title, const char *folder);
	static const char* saveFileDialog(const char *title, const char *folder);

};

#endif // DICTAC_GUI_FILEUTIL_H__
