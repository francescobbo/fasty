#include <iostream>
#include <cerrno>
#include <getopt.h>
#include <unistd.h>
#include <csignal>
#include <openssl/ssl.h>

#include "../utils/errno_exception.h"
#include "../utils/file.h"
#include "../utils/dir.h"
#include "server_socket.h"
#include "http_server.h"

using namespace std;

int https_server_port = 443;
int http_server_port = 80;
bool disable_ssl = false;

String pid_file = "tmp/pids/server.pid";
const SSL_METHOD *ssl_method = nullptr;
SSL_CTX *ssl_ctx = nullptr;

void parse_arguments(int argc, char *argv[]) {
	static struct option long_options[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "https-port", required_argument, NULL, 'p' },
		{ "http-port", required_argument, NULL, 'P' },
		{ "daemon", no_argument, NULL, 'd' },
		{ "pid", required_argument, NULL, 'f' },
		{ "no-ssl", no_argument, NULL, 's' },
		{ 0, 0, 0, 0 }
	};

	while (true) {
		opterr = 0;
		int opt = getopt_long(argc, argv, "hdsp:P:", long_options, NULL);
		if (opt == -1)
			break;

		switch (opt) {
			case 'h':
				cout << "Usage: ./fasty [options]" << endl;
				cout << "    -p, --https-port=port          Runs the HTTPS server on the specified port." << endl;
				cout << "        --http-port=port           Runs the HTTP server on the specified port." << endl;
				cout << "        Defaults: 443 and 80 when run as root, 5001 and 5000 otherwise." << endl << endl;
				cout << "        --no-ssl                   Disable HTTPS." << endl;
				cout << "    -d, --daemon                   Run the server as a daemon." << endl;
				cout << "    -P, --pid=pid                  Specified the PID file." << endl;
				cout << "                                   Default: tmp/pids/server.pid" << endl;
				cout << endl;
				cout << "    -h, --help                     Show this help message." << endl;
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
				https_server_port = String(optarg).to_i();
				break;
			case 'P':
				http_server_port = String(optarg).to_i();
				break;
			case 'f':
				pid_file = String(optarg);
				break;
			case 's':
				disable_ssl = true;
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

	if (ssl_ctx)
		SSL_CTX_free(ssl_ctx);

	delete_pid_file();
	exit(0);
}

#if ENABLE_HTTP2
/**
 * This callback method is called by OpenSSL when a client sends an ALPN
 * request with his supported methods.
 * The client methods are stored as a length-prefixed string array.
 * The server should put his supported methods (among the client ones) in @out.
 *
 * This is the only legitimate way to negotiate HTTP2 over TLS.
 */
int alpn_callback(SSL *s, const unsigned char **out, unsigned char *outlen, const unsigned char *in, unsigned int inlen, void *arg) {
	for (unsigned int i = 0; i < inlen; i += in[i] + 1)
	{
		String protocol = String((const char *) &in[i + 1]).substr(0, in[i]);

		/*
		 * Most draft implementations either declare HTTP2 with "h2-something"
		 * or "HTTP-draft-something", where "h2" whill be the ufficial protocol
		 * name.
		 */
		if (protocol.substr(0, 2) == "h2" || protocol.substr(0, 11) == "HTTP-draft-") {
			/* Report the protocol as a length prefixed array in @out */
			*out = (unsigned char *) in + i + 1;
			*outlen = in[i];

			/* Inform OpenSSL of the success */
			return SSL_TLSEXT_ERR_OK;
		}
	}

	/*
	 * Inform OpenSSL that a common protocol was not found (clients will fall
	 * back to HTTP/1.1)
	 */
	return SSL_TLSEXT_ERR_NOACK;
}
#endif

void init_openssl() {
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ssl_method = TLSv1_2_server_method();
	ssl_ctx = SSL_CTX_new(ssl_method);

#if ENABLE_HTTP2
	SSL_CTX_set_alpn_select_cb(ssl_ctx, alpn_callback, nullptr);
#endif

	if (!ssl_ctx) {
		cout << "Could not create SSL Context. Exiting..." << endl;
		exit(0);
	}

	if (!SSL_CTX_use_certificate_file(ssl_ctx, "server.crt", SSL_FILETYPE_PEM)) {
		cout << "Could not load Server Certificate file. Exiting..." << endl;
		exit(0);
	}

	if (!SSL_CTX_use_PrivateKey_file(ssl_ctx, "server.key", SSL_FILETYPE_PEM)) {
		cout << "Could not load Server Private Key file. Exiting..." << endl;
		exit(0);
	}
}

int main(int argc, char *argv[]) {
	if (geteuid() != 0) {
		https_server_port = 5001;
		http_server_port = 5000;
	}

	parse_arguments(argc, argv);

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	init_openssl();

	try {
		create_pid_file();

		ServerSocket s(disable_ssl ? http_server_port : https_server_port, !disable_ssl);

		if (disable_ssl)
			cout << "Starting HTTP server on port " << http_server_port << endl;
		else
			cout << "Starting HTTPS server on port " << https_server_port << endl;

		while (true) {
			ClientSocket client = s.next();

			pthread_t tid;
			pthread_create(&tid, NULL, (void *(*)(void *)) &HttpServer::InitThread, (void *) new ClientSocket(client));
		}
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}

	return 0;
}
