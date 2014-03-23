#include <map>
#include "string.h"

std::map<int, String> http_status_codes = {
	{ 100, "Continue" },
	{ 101, "Switching Protocols" },
	
	{ 200, "OK" },
	{ 201, "Created" },
	{ 202, "Accepted" },
	{ 203, "Non-Authoritative Information" },
	{ 204, "No Content" },
	{ 205, "Reset Content" },
	{ 206, "Partial Content" },

	{ 300, "Multiple Choices" },
	{ 301, "Moved Permanently" },
	{ 302, "Found" },
	{ 303, "See Other" },
	{ 304, "Not Modified" },
	{ 305, "Use Proxy" },
	{ 307, "Temporary Redirect" },

	{ 400, "Bad Request" },
	{ 401, "Unauthorized" },
	{ 402, "Payment Required" },
	{ 403, "Forbidden" },
	{ 404, "Not Found" },
	{ 405, "Method Not Allowed" },
	{ 406, "Not Acceptable" },
	{ 407, "Proxy Authentication Required" },
	{ 408, "Request Timeout" },
	{ 409, "Conflict" },
	{ 410, "Gone" },
	{ 411, "Length Required" },
	{ 412, "Precondition Failed" },
	{ 413, "Request Entity Too Large" },
	{ 414, "Request-URI Too Long" },
	{ 415, "Unsupported Media Type" },
	{ 416, "Requested Range Not Satisfiable" },
	{ 417, "Expectation Failed" },

	{ 500, "Internal Server Error" },
	{ 501, "Not Implemented" },
	{ 502, "Bad Gateway"},
	{ 503, "Service Unavailable" },
	{ 504, "Gateway Timeout" },
	{ 505, "HTTP Version Not Supported" }
};

std::map<int, String> http_status_descriptions = {
	{ 400, "The request could not be understood by the server due to malformed syntax." },
	{ 403, "The server understood the request, but is refusing to fulfill it." },
	{ 404, "The server has not found anything matching the Request-URI." },
	{ 405, "The method specified in the Request-Line is not allowed for the resource identified by the Request-URI." },
	{ 406, "The resource identified by the request is only capable of generating response entities which have content characteristics not acceptable according to the accept headers sent in the request." },
	{ 408, "The client did not produce a request within the time that the server was prepared to wait." },
	{ 411, "The server refuses to accept the request without a defined Content- Length." },
	{ 415, "Unsupported Media Type" },
	{ 416, "Requested Range Not Satisfiable" },
	{ 417, "Expectation Failed" },

	{ 500, "The server encountered an unexpected condition which prevented it from fulfilling the request." },
	{ 501, "The server does not support the functionality required to fulfill the request. This is the appropriate response when the server does not recognize the request method and is not capable of supporting it for any resource." },
	{ 503, "The server is currently unable to handle the request due to a temporary overloading or maintenance of the server." },
	{ 505, "HTTP Version Not Supported" }
};
