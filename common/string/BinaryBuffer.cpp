#include "BinaryBuffer.hpp"

namespace String {

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

bool BinaryBuffer::operator==(const BinaryBuffer &arr) const {
	return this->size() == arr.size() && \
		memcmp(this->data(), arr.data(), this->size()) == 0;
}

bool BinaryBuffer::operator==(const std::vector<char> &vec) const {
	return this->size() == vec.size() && \
		memcmp(this->data(), vec.data(), this->size()) == 0;
}

bool BinaryBuffer::operator==(const std::string &str) const {
	return this->size() == str.size() && \
		memcmp(this->data(), str.data(), this->size()) == 0;
}

bool BinaryBuffer::operator==(const char *str) const {
	return this->size() == strlen(str) && \
		memcmp(this->data(), str, this->size()) == 0;
}

bool BinaryBuffer::operator!=(const BinaryBuffer &arr) const {
	return !(*this == arr);
}

bool BinaryBuffer::operator!=(const std::vector<char> &vec) const {
	return !(*this == vec);
}

bool BinaryBuffer::operator!=(const std::string &str) const {
	return !(*this == str);
}

bool BinaryBuffer::operator!=(const char *str) const {
	return !(*this == str);
}

std::string BinaryBuffer::str() const {
	return std::string(begin(), end());
}

std::vector<char> BinaryBuffer::vec() const {
	return *this;
}

char *BinaryBuffer::c_str() {
	push_back('\0');
	pop_back();
	return data();
}

BinaryBuffer BinaryBuffer::readLine() {
	BinaryBuffer line = "";

	for (std::vector<char>::iterator it = begin(); it != end(); ++it) {
		if (*it == '\n') {
			line.insert(line.end(), begin(), it + 1);
			erase(begin(), it + 1);
			break;
		}
	}
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
	return std::string::npos;
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

}