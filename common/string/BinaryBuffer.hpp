#ifndef BINARYBUFFER_HPP
# define BINARYBUFFER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class BinaryBuffer : public std::vector<char> {
public:
	BinaryBuffer();
	BinaryBuffer(iterator begin, iterator end);
	BinaryBuffer(const std::string &str);
	BinaryBuffer(const char *str);
	BinaryBuffer(const char *str, size_t len);
	BinaryBuffer(const std::vector<char> &vec);
	BinaryBuffer(const BinaryBuffer &arr);

	BinaryBuffer &operator=(const BinaryBuffer &arr);
	BinaryBuffer &operator=(const std::vector<char> &vec);
	BinaryBuffer &operator=(const std::string &str);
	BinaryBuffer &operator=(const char *str);
	BinaryBuffer &operator+=(const BinaryBuffer &arr);
	BinaryBuffer &operator+=(const std::vector<char> &vec);
	BinaryBuffer &operator+=(const std::string &str);
	BinaryBuffer &operator+=(const char *str);
	BinaryBuffer &operator+=(char c);


	BinaryBuffer &operator<<(const BinaryBuffer &arr);
	BinaryBuffer &operator<<(const std::vector<char> &vec);
	BinaryBuffer &operator<<(const std::string &str);
	BinaryBuffer &operator<<(const char *str);
	BinaryBuffer &operator<<(char c);
	std::string str() const;
	std::vector<char> vec() const;

	BinaryBuffer readLine();

	BinaryBuffer subStr(size_t pos, size_t len);

	size_t find(char c);

	void remove(size_t pos, size_t len);
	void clear();


};

std::ostream &operator<<(std::ostream &os, const BinaryBuffer &buffer);

#endif