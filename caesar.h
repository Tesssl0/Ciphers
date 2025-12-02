// caesar.h
#ifndef CAESAR_H
#define CAESAR_H

#include <string>

bool validateTextCaesar(const std::string& text, const std::string& language);
std::string encryptCaesarCipher(const std::string& text, int key, const std::string& language);
std::string decryptCaesarCipher(const std::string& text, int key, const std::string& language);
void runCaesarCipher(); 

#endif