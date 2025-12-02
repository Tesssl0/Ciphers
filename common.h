// common.h
#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>

bool validateMixedText(const std::string& text);
bool fileExists(const std::string& filename);
std::string readFile(const std::string& filename);
bool writeToFile(const std::string& filename, const std::string& content);
bool createFile(const std::string& filename, const std::string& content);

#endif