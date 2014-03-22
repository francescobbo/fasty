#include "blob.h"
#include "file.h"
#include "string.h"
#include "errno_exception.h"

#include <sys/stat.h>
#include <sys/unistd.h>

File::File(const String &file_name, const String &mode) {
	file = fopen(file_name, mode);
}

File::~File() {
	if (file)
		fclose(file);
}

int File::atime(const String &file_name) {
	struct stat buf;

	if (stat(file_name, &buf) < 0)
		throw ErrnoException("Could not stat " + file_name, errno);

	return buf.st_atime;
}

int File::ctime(const String &file_name) {
	struct stat buf;

	if (stat(file_name, &buf) < 0)
		throw ErrnoException("Could not stat " + file_name, errno);

	return buf.st_ctime;
}

static const char *last_separator(const char *path) {
	const char *last = NULL;

	while (*path) {
		if (*path == '/') {
			const char *tmp = path++;
			while (*path == '/')
				path++;
			if (!*path)
				break;
			last = tmp;
		} else {
			path++;
		}
	}

	return last;
}

String File::dirname(const String &path) {
	const char *name = path;
	const char *root = name;

	while (*root == '/')
		root++;

	if (root > name + 1)
		name = root - 1;

	const char *last = last_separator(root);
	if (!last)
		last = root;

	if (last == name)
		return ".";

	return String(name, 0, last - name);
}

bool File::exists(const String &file_name) {
	struct stat buf;

	if (stat(file_name, &buf) < 0)
		return false;

	return true;
}

bool File::is_directory(const String &file_name) {
	struct stat buf;

	if (stat(file_name, &buf) < 0)
		return false;

	if (S_ISDIR(buf.st_mode))
		return true;

	return false;
}

bool File::is_file(const String &file_name) {
	struct stat buf;

	if (stat(file_name, &buf) < 0)
		return false;

	if (S_ISREG(buf.st_mode))
		return true;

	return false;
}

bool File::is_readable(const String &file_name) {
	if (access(file_name, R_OK) < 0)
		return false;
	return true;
}

//Time File::mtime(const String &file_name) {
int File::mtime(const String &file_name) {
	struct stat buf;
	
	if (stat(file_name, &buf) < 0)
		throw ErrnoException("Could not stat " + file_name, errno);

	long sec = buf.st_mtime;
/*
#if defined(HAVE_STRUCT_STAT_ST_MTIM)
	int nsec = st->st_mtim.tv_nsec;
#elif defined(HAVE_STRUCT_STAT_ST_MTIMESPEC)
	int nsec = st->st_mtimespec.tv_nsec;
#elif defined(HAVE_STRUCT_STAT_ST_MTIMENSEC)
	int nsec = st->st_mtimensec;
#else
	int nsec = 0;
#endif
*/

	return sec; //Time::at(sec, nsec);
}

long File::size(const String &file_name) {
	struct stat buf;
	
	if (stat(file_name, &buf) < 0)
		throw ErrnoException("Could not stat " + file_name, errno);

	return buf.st_size;
}

void File::close() {
	fclose(file);
	file = nullptr;
}

Blob File::read(int length) {
	if (length == -1)
		length = size() - tell();

	Blob result;
	result.ensure_capacity(length);

	int read = fread(result.ptr(), 1, length, file);
	result.size(read);

	return result;
}

long File::size() const {
	struct stat buf;
	int fd = fileno(file);

	if (fstat(fd, &buf)  == -1)
		throw ErrnoException("Could not stat " + file_name, errno);

	return buf.st_size;
}

long File::tell() const {
	return ftell(file);
}

