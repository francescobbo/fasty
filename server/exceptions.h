#pragma once

#include <stdexcept>

class ConnectionClosedException : std::exception {
public:
	ConnectionClosedException() {}

	const char *what() const noexcept {
		return "Connection closed by remote host.";
	}
};