#include "dir.h"
#include "file.h"
#include "errno_exception.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

bool Dir::exists(const String &path) {
	return File::is_directory(path);
}

void Dir::mkdir(const String &path, int mode) {
	if (::mkdir(path, mode) < 0)
		throw ErrnoException(String("Unable to create directory ") + path, errno);
}

void Dir::mkdir_p(const String &path, int mode) {
	auto dirs = path.split('/');
	auto it = dirs.begin();
	String cur = ".";

	if (dirs[0].is_empty()) {
		cur = "";
		it++;
	}

	for (; it != dirs.end(); it++) {
		cur += String("/") + *it;
		if (exists(cur))
			continue;

		if (::mkdir(cur, mode) < 0)
			throw ErrnoException(String("Unable to create directory ") + cur, errno);
	}
}

void Dir::rmdir(const String &path) {
	if (::rmdir(path) < 0)
		throw ErrnoException(String("Unable to remove directory ") + path, errno);
}

