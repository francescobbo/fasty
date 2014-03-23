#include "uri.h"

Uri::Uri(const String &uri) {
	int scheme_separator = uri.index("://");
	int path_separator = 0;
	_port = 0;

	if (scheme_separator > 0) {
		// Schemed uri
		_scheme = uri.substr(0, scheme_separator);

		path_separator = uri.index('/', scheme_separator + 3);

		_host = uri.substr(scheme_separator + 3, path_separator - (scheme_separator + 3));
		if (_host.index(':') >= 0) {
			auto elements = _host.split(':');
			_host = elements[0];
			_port = elements[1].to_i();
		}
	}

	int query = uri.index('?', path_separator);
	int fragment;
	int path_end = query;
	
	if (query >= 0)
		fragment = uri.index('#', query);
	else
		path_end = fragment = uri.index('#', path_separator);

	_path = uri.substr(path_separator, path_end - path_separator);

	if (query >= 0) {
		if (fragment > 0)
			_query = uri.substr(query + 1, fragment - query - 1);
		else
			_query = uri.substr(query + 1);
	}

	if (fragment >= 0)
		_fragment = uri.substr(fragment + 1);

	// Set default port for known schemes
	if (_port == 0 and _scheme != "") {
		if (_scheme.casecmp("http"))
			_port = 80;
		else if (_scheme.casecmp("https"))
			_port = 443;
		else if (_scheme.casecmp("ftp"))
			_port = 21;
	}
}

String Uri::scheme() const {
	return _scheme;
}

String Uri::host() const {
	return _host;
}

String Uri::host_with_port(bool skip_default) const {
	if (_host != "") {
		if (skip_default and is_default_port())
			return _host;
		else
			return _host + ":" + _port;
	} else
		return "";
}

int Uri::port() const {
	return _port;
}

String Uri::path() const {
	return _path;
}

String Uri::query() const {
	return _query;
}

String Uri::fragment() const {
	return _fragment;
}

String Uri::request_uri() const {
	if (_query != "")
		return _path + '?' + _query;
	else
		return _path;
}

bool Uri::is_default_port() const {
	if (_port == 0 or _scheme == "")
		return false;

	if (_port == 80 and _scheme.casecmp("http"))
		return true;
	if (_port == 443 and _scheme.casecmp("https"))
		return true;
	if (_port == 21 and _scheme.casecmp("ftp"))
		return true;

	return false;
}
