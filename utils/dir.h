#pragma once

#include "string.h"

class Dir {
public:
	static bool exists(const String &path);
	static void mkdir(const String &path, int mode = 0777);
	static void mkdir_p(const String &path, int mode = 0777);
	static void rmdir(const String &path);
};

