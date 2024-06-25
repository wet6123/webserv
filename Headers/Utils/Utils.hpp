#ifndef UTILS_HPP
#define UTILS_HPP

#include "../../Headers/Utils/Exception.hpp"
#include "Define.hpp"
#include "StringReader.hpp"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#define TRIM_SPACE " \n\v\t"

bool existFile(std::string path);
bool existDir(std::string path);
std::string trim(std::string s);
std::string trimComment(std::string s);
bool stringStartWith(std::string str, std::string find);
std::string strjoin(std::string s1, std::string s2);
std::string strjoin(std::string s1, std::string s2, std::string s3);
std::string strjoin(std::string s1, std::string s2, std::string s3,
                    std::string s4);
int stringToInt(std::string str);
std::string itos(int n);
int findStartBlockPos(StringReader &sr);
int findEndBlockPos(StringReader &sr);
std::string fileToString(std::string path);
std::string fileToString(std::fstream &file);
t_vecString strSplit(std::string str, char delimiter, bool mustTrim);
DictElem makeDictElem(std::string str);
#endif