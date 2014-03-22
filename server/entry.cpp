#include <iostream>
#include <cerrno>

#include "../utils/errno_exception.h"
#include "server_socket.h"

using namespace std;

int main() {
	try {
		ServerSocket s(4000);
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}

	return 0;
}
