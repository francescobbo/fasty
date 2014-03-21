#include "string.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

String::String(const char *str) {
	int length = strlen(str) + 1;

	data = new char[length];
	strcpy(data, str);
	data[length - 1] = '\0';

	capacity = length;
}

String::String(const String &copy) : String(copy.data) {
}

String::String(const std::string &copy) : String(copy.c_str()) {
}

String::String(String &&move) {
	data = move.data;
	capacity = move.capacity;
	move.data = nullptr;
}

String::String(const char *str, int start, int length) {
	// Skip the first @start characters
	str += start;

	// Ensure that length does not overflow
	int str_length = strlen(str);
	length = length > str_length ? str_length : length;

	data = new char[length + 1];
	strncpy(data, str, length);
	data[length] = '\0';

	capacity = length + 1;
}

String::~String() {
	if (data)
		delete[] data;
}

size_t String::length() const {
	return size();
}

size_t String::size() const {
	return strlen(data);
}

String String::capitalize() const {
	String copy(*this);
	return copy.capitalize_s();
}

String &String::capitalize_s() {
	if (length() > 0) {
		lowercase_s();
		data[0] = toupper(data[0]);
	}
		
	return *this;
}

bool String::casecmp(const String &str) const {
	int i = 0;
	for (; data[i] != '\0' and str.data[i] != '\0'; i++) {
		if (tolower(data[i]) != tolower(str.data[i]))
			return false;
	}

	return data[i] == str.data[i] and data[i] == '\0';
}

void String::clear() {
	data[0] = 0;
}

bool String::contains(const String &str) const {
	return index(str) >= 0;
}

String String::erase(int start, int length) const {
	String copy(*this);
	return copy.erase_s(start, length);
}

String &String::erase_s(int start, int length) {
	int orig_length = strlen(data);

	if (start < 0)
		start = orig_length + start;

	if (length < 0)
		return *this;

	if (start + length > orig_length)
		length = orig_length - start;

	for (int i = start; i < orig_length - length + 1; i++)
		data[i] = data[i + length];
		
	return *this;
}

String String::format(const String &fmt, ...) {
	int n = fmt.length() * 2;
	std::unique_ptr<char[]> formatted;

	va_list ap;
	while (true) {
		formatted.reset(new char[n]);
		strcpy(&formatted[0], fmt);
		
		va_start(ap, fmt);
		int result = vsnprintf(&formatted[0], n, fmt, ap);
		va_end(ap);

		if (result < 0 || result >= n)
			n += abs(result - n + 1);
		else
			break;
	}
	
	return String(formatted.get());
}

int String::index(char c, int skip) const {
	char *pos = strchr(data + skip, c);
	long index = (long) (pos - data);
	return index >= 0 ? index : -1;
}

int String::index(const String &str, int skip) const {
	char *pos = strstr(data + skip, str.data);
	long index = (long) (pos - data);
	return index >= 0 ? index : -1;
}

bool String::is_empty() const {
	return length() == 0;
}

String String::lowercase() const {
	String copy(*this);
	return copy.lowercase_s();
}

String &String::lowercase_s() {
	for (int i = 0; data[i] != '\0'; i++)
		data[i] = tolower(data[i]);
		
	return *this;
}

String String::replace(const String &find, const String &replace) const {
	String ret(*this);
	
	for (int pos = 0; (pos = ret.index(find, pos)) >= 0; pos += replace.length())
		ret = ret.replace(pos, find.length(), replace);
	
	return ret;
}

String String::replace(int start, int length, const String &replace) const {
	String ret = substr(0, start);
	ret += replace;
	return ret + substr(start + length);
}

vector<String> String::split(char delimiter, bool ignore_empty) const {
	vector<String> elems;
	stringstream stream(data);
	
	string token;
	while (getline(stream, token, delimiter)) {
		if (!(token == "" and ignore_empty))
			elems.push_back(String(token));
	}

	return elems;
}

vector<String> String::split(const String &delimiter) const {
	String copy(*this);
	vector<String> elems;

	int pos;
	while ((pos = copy.index(delimiter)) != -1) {
		String token = copy.substr(0, pos);
		elems.push_back(token);
		copy.erase(0, pos + delimiter.length());
	}

	// Remainings characters
	elems.push_back(copy);
	return elems;
}

String String::substr(int start, int length) const {
	if (start < 0 and length < 0)
		return "";

	int cur_length = size();

	// Normalize start and length
	if (start < 0)
		start = cur_length + start;

	if (start > cur_length)
		return "";

	if (length < 0)
		length = cur_length + 1 + length;

	if (start + length > cur_length)
		length = cur_length - start;

	return String(data, start, length);
}

String String::uppercase() const {
	String copy(*this);
	return copy.uppercase_s();
}

String &String::uppercase_s() {
	for (int i = 0; data[i] != '\0'; i++)
		data[i] = toupper(data[i]);
		
	return *this;
}

String String::trim() const {
	String copy(*this);
	return copy.trim_s();
}


String &String::trim_s() {
	int start = 0;
	int end = strlen(data) - 1;
	
	if (end < 0)
		return *this;
	
	while (data[start] != '\0' and isspace(data[start]))
		start++;
		
	while (end > 0 and isspace(data[end]))
		end--;
		
	memmove(data, data + start, end - start + 1);
	data[end - start + 1] = '\0';
	
	return *this;
}

int String::to_i() const {
	return strtol(data, NULL, 10);
}

String &String::operator=(const String &copy) {
	if (&copy == this)
		return *this;

	if (data)
		delete[] data;

	data = new char[copy.capacity];
	capacity = copy.capacity;
	strcpy(data, copy.data);

	return *this;
}

String &String::operator=(String &&move) {
	if (&move == this)
		return *this;

	data = move.data;
	capacity = move.capacity;
	move.data = nullptr;

	return *this;
}

bool String::operator==(const String &other) const {
	return !strcmp(data, other.data);
}

bool String::operator==(const char *other) const {
	return !strcmp(data, other);
}

bool String::operator<(const String &other) const {
	return strcmp(data, other.data) < 0;
}

bool String::operator!=(const String &other) const {
	return !operator==(other);
}

bool String::operator!=(const char *other) const {
	return !operator==(other);
}

String &String::operator+=(const char *str) {
	int cur_length = strlen(data);
	int new_length = cur_length + strlen(str) + 1;

	ensure_capacity(new_length);

	strcpy(data + cur_length, str);
	data[new_length - 1] = '\0';

	return *this;
}

String &String::operator+=(char c) {
	int cur_length = strlen(data);

	ensure_capacity(cur_length + 2);

	data[cur_length] = c;
	data[cur_length + 1] = '\0';

	return *this;
}

String &String::operator+=(const String &str) {
	return (*this += str.data);
}

String &String::operator+=(int value) {
	char buff[20];
	sprintf(buff, "%d", value);
	return (*this += buff);
}

String String::operator+(const String &str) const {
	String copy(*this);
	copy += str.data;
	return copy;
}

String String::operator+(char c) const {
	String copy(*this);
	copy += c;
	return copy;
}

String String::operator+(const char *str) const {
	String copy(*this);
	copy += str;
	return copy;
}

String String::operator+(int value) const {
	String copy(*this);
	copy += value;
	return copy;
}

String::operator const char *() const {
	return data;
}

char String::operator[](int index) const {
	return data[index];
}

char &String::operator[](int index) {
	return data[index];
}

void String::ensure_capacity(int minimum) {
	if (minimum > capacity) {
		char *temp = new char[minimum];
		strcpy(temp, data);
		temp[strlen(data)] = '\0';
		
		delete[] data;
		data = temp;

		capacity = minimum;
	}
}

bool operator==(const char *str1, const String &str2) {
	return str2 == str1;
}

String operator+(const char *str1, const String &str2) {
	String s(str1);
	return s + str2;
}

