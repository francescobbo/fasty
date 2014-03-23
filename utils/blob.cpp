#include "blob.h"
#include "string.h"

#include <zlib.h>
#include <cstring>
#include <stdio.h>

Blob::Blob() {
	data = nullptr;
	_size = 0;
	capacity = 0;
}

Blob::Blob(const char *buffer, int length) {
	data = new unsigned char[length];
	_size = capacity = length;
	memcpy(data, buffer, length);
}

Blob::Blob(const Blob &copy) : _size(copy._size), capacity(copy.capacity) {
	if (copy.data) {
		data = new unsigned char[capacity];
		memcpy(data, copy.data, _size);
	} else
		data = nullptr;
}

Blob::Blob(const String &copy) : Blob(copy, copy.size()) {
}

Blob::Blob(Blob &&move) : data(move.data), _size(move._size), capacity(move.capacity) {
	move.data = nullptr;
}

Blob::~Blob() {
	if (data)
		delete[] data;
}

bool Blob::contains(unsigned char c) const {
	return index(c) >= 0;
}

bool Blob::contains(const String &str) const {
	return index(str) >= 0;
}

bool Blob::contains(const Blob &blob) const {
	return index(blob) >= 0;
}

int Blob::index(unsigned char c, int start) const {
	for (size_t i = start; i < _size; i++) {
		if (data[i] == c)
			return i;
	}

	return -1;
}

int Blob::index(const String &str, int start) const {
	size_t str_length = str.size();
	if (str_length > _size)
		return -1;

	for (size_t i = start; i < _size - str_length + 1; i++) {
		if (!memcmp(&data[i], str, str_length))
			return i;
	}

	return -1;
}

int Blob::index(const Blob &blob, int start) const {
	if (blob._size > _size)
		return -1;

	for (size_t i = start; i < _size - blob._size; i++) {
		if (!memcmp(&data[i], blob.ptr(), blob._size))
			return i;
	}

	return -1;
}

Blob Blob::substr(int start, int length) const {
	if (start < 0 and length < 0)
		return Blob();

	int cur_length = _size;

	// Normalize start and length
	if (start < 0)
		start = cur_length + start;

	if (start > cur_length)
		return Blob();

	if (length < 0)
		length = cur_length + 1 + length;

	if (start + length > cur_length)
		length = cur_length - start;

	return Blob((const char *) data + start, length);
}

Blob &Blob::operator=(const Blob &copy) {
	if (&copy == this)
		return *this;

	if (!data)
		data = new unsigned char[copy.capacity];
	else {
		if (capacity < copy.capacity) {
			delete[] data;
			data = new unsigned char[copy.capacity];
		}
	}

	_size = copy._size;
	memcpy(data, copy.data, _size);

	return *this;
}

bool Blob::operator==(const String &str) const {
	size_t i = 0;
	for (; i < _size; i++) {
		if (!str[i])
			return false;

		if (data[i] != str[i])
			return false;
	}

	if (str[i] != '\0')
		return false;

	return true;
}

bool Blob::operator==(const Blob &blob) const {
	if (_size != blob._size)
		return false;

	return !memcmp(data, blob.data, _size);
}

const unsigned char *Blob::ptr() const {
	return data;
}

unsigned char *Blob::ptr() {
	return data;
}

size_t Blob::size() const {
	return _size;
}

void Blob::size(size_t size) {
	ensure_capacity(size);
	_size = size;
}

Blob &Blob::operator+=(char c) {
	ensure_capacity(_size + 1);
	data[_size++] = c;
	return *this;
}

Blob &Blob::operator+=(const Blob &blob) {
	append((const char *) blob.ptr(), blob.size());
	return *this;
}

Blob &Blob::operator+=(const String &str) {
	append((const char *) str, str.length());
	return *this;
}

Blob &Blob::append(const char *buffer, int length) {
	ensure_capacity(_size + length);
	memcpy(&data[_size], buffer, length);
	_size += length;

	return *this;
}

Blob Blob::operator+(const String &str) const {
	Blob ret(*this);
	ret += str;
	return ret;
}

Blob Blob::operator+(const Blob &blob) const {
	Blob ret(*this);
	ret += blob;
	return ret;
}

const unsigned char Blob::operator[](int index) const {
	return data[index];
}

unsigned char &Blob::operator[](int index) {
	return data[index];
}

void Blob::ensure_capacity(int minimum) {
	if (minimum > capacity) {
		unsigned char *temp = new unsigned char[minimum];
		capacity = minimum;

		if (data) {
			memcpy(temp, data, _size);
			delete[] data;
		}

		data = temp;
	}
}

void Blob::save(String fn) {
	FILE *f = fopen(fn, "wb");
	fwrite(data, _size, 1, f);
	fclose(f);
}

