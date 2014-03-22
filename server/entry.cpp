#include <iostream>
#include <cerrno>
#include <getopt.h>
#include <unistd.h>
#include <csignal>

#include "../utils/errno_exception.h"
#include "../utils/file.h"
#include "../utils/dir.h"
#include "server_socket.h"

using namespace std;

int server_port = 5000;
String pid_file = "tmp/pids/server.pid";

void parse_arguments(int argc, char *argv[]) {
	static struct option long_options[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "port", required_argument, NULL, 'p' },
		{ "daemon", no_argument, NULL, 'd' },
		{ "pid", required_argument, NULL, 'P' },
		{ 0, 0, 0, 0 }
	};

	while (true) {
		opterr = 0;
		int opt = getopt_long(argc, argv, "hdp:P:", long_options, NULL);
		if (opt == -1)
			break;

		switch (opt) {
			case 'h':
				cout << "Usage: ./fasty [options]" << endl;
				cout << "	-p, --port=port                  Runs the server on the specified port." << endl;
				cout << "                                    Default: 5000" << endl;
				cout << "	-d, --daemon                     Run the server as a daemon." << endl;
				cout << "	-P, --pid=pid                    Specified the PID file." << endl;
				cout << "                                    Default: tmp/pids/server.pid" << endl;
				cout << endl;
				cout << "	-h, --help                       Show this help message." << endl;
				exit(0);
				break;
			case 'd': {
				cout << "[ Running as a daemon ]" << endl;

				pid_t pid = fork();
				if (pid < 0)
					exit(-1);
				else if (pid > 0)
					exit(0);	// Exit from the parent

				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				close(STDERR_FILENO);
				break;
			}
			case 'p':
				server_port = String(optarg).to_i();
				break;
			case 'P':
				pid_file = String(optarg);
				break;
		}
	}
}

void create_pid_file() {
	if (!Dir::exists(File::dirname(pid_file)))
		Dir::mkdir_p(File::dirname(pid_file), 0755);

	File f(pid_file, "w");
	f.write(String("") + getpid());
	f.close();
}

void delete_pid_file() {
	File::remove(pid_file);
}

void signal_handler(int signal) {
	cout << "Terminating..." << endl;
	delete_pid_file();
	exit(0);
}

int main(int argc, char *argv[]) {
	parse_arguments(argc, argv);

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	try {
		create_pid_file();

		ServerSocket s(server_port);

		while (true) {
			ClientSocket client = s.next();
		}
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}

	return 0;
}
