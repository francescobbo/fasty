#include <stdexcept>
#include <cstring>
#include "string.h"

class ErrnoException : public std::exception {
public:
	ErrnoException(const std::string &what, int code) {
		msg = what + ": " + strerror(code) + ".";
		this->_errno = code;
	}
	
	const char *what() const noexcept {
		return msg.c_str();
	}
	
private:
	std::string msg;
	int _errno;
};
