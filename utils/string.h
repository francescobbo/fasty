#pragma once

#include <string>
#include <vector>

class String {
public:
	String(const char *str = "");
	String(const String &copy);
	String(String &&move);
	String(const std::string &copy);
	String(const char *str, int start, int length);
	~String();

	size_t length() const;
	size_t size() const;

	String capitalize() const;
	String &capitalize_s();
	bool casecmp(const String &str) const;
	void clear();
	bool contains(const String &str) const;
	String erase(int start, int length) const;
	String &erase_s(int start, int length);
	static String format(const String &fmt, ...);
	int index(char c, int skip = 0) const;
	int index(const String &str, int skip = 0) const;
	bool is_empty() const;
	String lowercase() const;
	String &lowercase_s();
	String replace(const String &find, const String &replace) const;
	String replace(int start, int length, const String &replace) const;
	std::vector<String> split(char delimiter = ' ', bool ignore_empty = false) const;
	std::vector<String> split(const String &delimiter) const;
	String substr(int start, int length = -1) const;
	String uppercase() const;
	String &uppercase_s();
	String trim() const;
	String &trim_s();

	int to_i() const;

	String &operator=(const String &copy);
	String &operator=(String &&move);

	bool operator==(const String &other) const;
	bool operator==(const char *other) const;
	bool operator!=(const String &other) const;
	bool operator!=(const char *other) const;
	bool operator<(const String &other) const;
	String &operator+=(const char *str);
	String &operator+=(char c);
	String &operator+=(const String &str);
	String &operator+=(int value);
	String operator+(const String &str) const;
	String operator+(char c) const;
	String operator+(const char *str) const;
	String operator+(int value) const;
	operator const char *() const;
	operator std::string() const;
	char operator[](int index) const;
	char &operator[](int index);
	
	void ensure_capacity(int minimum);
	
private:
	char *data;
	int capacity;
};

bool operator==(const char *str1, const String &str2);
String operator+(const char *str1, const String &str2);
