#pragma once

#include <stdio.h>
#include "time.h"

class Blob;
class String;

class File {
public:
	File(const String &file_name, const String &mode);
	~File();

	static int atime(const String &file_name);
	static int ctime(const String &file_name);
	static String dirname(const String &path);
	static bool exists(const String &file_name);
	static bool is_directory(const String &file_name);
	static bool is_file(const String &file_name);
	static bool is_readable(const String &file_name);
//	static Time mtime(const String &file_name);
	static int mtime(const String &file_name);
	static long size(const String &file_name);

	int atime();
	void close();
	Blob read(int length = -1);
	long size() const;
	long tell() const;

private:
	FILE *file = nullptr;
};
