#pragma once

class String;

class Blob {
public:
	Blob();
	Blob(const char *buffer, int length);
	Blob(const Blob &copy);
	Blob(const String &copy);
	Blob(Blob &&move);
	~Blob();

	bool contains(unsigned char c) const;
	bool contains(const String &str) const;
	bool contains(const Blob &blob) const;

	int index(unsigned char c, int start = 0) const;
	int index(const String &str, int start = 0) const;
	int index(const Blob &blob, int start = 0) const;

	Blob &operator=(const Blob &copy);
	bool operator==(const Blob &blob) const;
	bool operator==(const String &str) const;

	Blob operator+(const Blob &blob) const;
	Blob operator+(const String &str) const;
	Blob &operator+=(char c);
	Blob &operator+=(const String &str);
	Blob &operator+=(const Blob &blob);
	Blob &append(const char *buffer, int length);

	void save(String filename);

	const unsigned char *ptr() const;
	unsigned char *ptr();
	
	const unsigned char operator[](int index) const;
	unsigned char &operator[](int index);

	int size() const;
	void size(int new_size);	// When creating a blob dynamically, you might want to alter it's size

	void ensure_capacity(int minimum);

private:
	unsigned char *data;
	int _size;
	int capacity;
};

