#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

std::string encryptVigenereCipher(const std::string& text, const std::string& key);
std::string decryptVigenereCipher(const std::string& text, const std::string& key);
void runVigenereCipher();

#endif