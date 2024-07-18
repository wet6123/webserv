#include "BinaryBuffer.hpp"

BinaryBuffer::BinaryBuffer() : std::vector<char>() {}

BinaryBuffer::BinaryBuffer(const std::string &str) : std::vector<char>(str.begin(), str.end()) {}

BinaryBuffer::BinaryBuffer(const char *str) : std::vector<char>(str, str + strlen(str)) {}

BinaryBuffer::BinaryBuffer(const char *str, size_t len) : std::vector<char>(str, str + len) {}

BinaryBuffer::BinaryBuffer(const std::vector<char> &vec) : std::vector<char>(vec) {}

BinaryBuffer::BinaryBuffer(const BinaryBuffer &arr) : std::vector<char>(arr) {}

BinaryBuffer::BinaryBuffer(iterator begin, iterator end) : std::vector<char>(begin, end) {}

BinaryBuffer &BinaryBuffer::operator=(const BinaryBuffer &arr) {
	if (this != &arr) {
		std::vector<char>::operator=(arr);
	}
	return *this;
}

BinaryBuffer &BinaryBuffer::operator=(const std::vector<char> &vec){
	std::vector<char>::operator=(vec);
	return *this;
}

BinaryBuffer &BinaryBuffer::operator=(const std::string &str) {
	std::vector<char>::operator=(std::vector<char>(str.begin(), str.end()));
	return *this;
}

BinaryBuffer &BinaryBuffer::operator=(const char *str) {
	std::vector<char>::operator=(std::vector<char>(str, str + strlen(str)));
	return *this;
}

BinaryBuffer &BinaryBuffer::operator+=(const BinaryBuffer &arr) {
	insert(end(), arr.begin(), arr.end());
	return *this;
}

BinaryBuffer &BinaryBuffer::operator+=(const std::vector<char> &vec) {
	insert(end(), vec.begin(), vec.end());
	return *this;
}

BinaryBuffer &BinaryBuffer::operator+=(const std::string &str) {
	insert(end(), str.begin(), str.end());
	return *this;
}

BinaryBuffer &BinaryBuffer::operator+=(const char *str) {
	insert(end(), str, str + strlen(str));
	return *this;
}

BinaryBuffer &BinaryBuffer::operator+=(char c) {
	push_back(c);
	return *this;
}

BinaryBuffer &BinaryBuffer::operator<<(const BinaryBuffer &arr) {
	insert(end(), arr.begin(), arr.end());
	return *this;
}

BinaryBuffer &BinaryBuffer::operator<<(const std::vector<char> &vec) {
	insert(end(), vec.begin(), vec.end());
	return *this;
}

BinaryBuffer &BinaryBuffer::operator<<(const std::string &str) {
	insert(end(), str.begin(), str.end());
	return *this;
}

BinaryBuffer &BinaryBuffer::operator<<(const char *str) {
	insert(end(), str, str + strlen(str));
	return *this;
}

BinaryBuffer &BinaryBuffer::operator<<(char c) {
	push_back(c);
	return *this;
}

std::string BinaryBuffer::str() const {
	return std::string(begin(), end());
}

std::vector<char> BinaryBuffer::vec() const {
	return *this;
}

BinaryBuffer BinaryBuffer::readLine() {
	BinaryBuffer line;

	for (std::vector<char>::iterator it = begin(); it != end(); ++it) {
		if (*it == '\n') {
			line.insert(line.end(), begin(), it);
			erase(begin(), it + 1);
			return line;
		}
	}
	line = *this;
	clear();
	return line;
}


BinaryBuffer BinaryBuffer::subStr(size_t pos, size_t len) {
	return BinaryBuffer(begin() + pos, begin() + pos + len);
}

size_t BinaryBuffer::find(char c) {
	for (size_t i = 0; i < size(); ++i) {
		if (at(i) == c) {
			return i;
		}
	}
	return -1;
}

void BinaryBuffer::remove(size_t pos, size_t len) {
	erase(begin() + pos, begin() + pos + len);
}

void BinaryBuffer::clear() {
	erase(begin(), end());
}

std::ostream & operator<<(std::ostream &os, const BinaryBuffer &buffer) {
	for (std::vector<char>::const_iterator it = buffer.begin(); it != buffer.end(); ++it) {
		os << *it;
	}
	return os;
}