#include <stdexcept>
#include <cstring>
#include "string.h"

class ErrnoException : public std::exception {
public:
	ErrnoException(const std::string &what, int _errno) {
		msg = what + ": " + strerror(_errno) + ".";
		this->_errno = _errno;
	}
	
	const char *what() const noexcept {
		return msg.c_str();
	}
	
private:
	std::string msg;
	int _errno;
};
