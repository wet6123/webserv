#ifndef UTILS_HPP
#define UTILS_HPP

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#define TRIM_SPACE " \\t\\n\\v"

bool existFile(std::string path);
bool existDir(std::string path);
std::string trim(std::string s);
std::string trimComment(std::string s);
bool stringStartWith(std::string str, std::string find);
std::string strjoin(std::string s1, std::string s2);
std::string strjoin(std::string s1, std::string s2, std::string s3);
std::string strjoin(std::string s1, std::string s2, std::string s3,
                    std::string s4);
std::string itos(int n);
int findStartBlockPos(std::stringstream &ss);
int findEndBlockPos(std::stringstream &ss);
std::string fileToString(std::string path);
std::string fileToString(std::fstream file);
std::vector<std::string> strSplit(std::string str, char delimiter);
#endif