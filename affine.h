// affine.h
#ifndef AFFINE_H
#define AFFINE_H

#include <string>

// Объявляем функции с разными именами чтобы избежать конфликтов
bool validateTextAffine(const std::string& text, const std::string& language);
int modInverseAffine(int a, int n);
std::string encryptAffineCipher(const std::string& text, int a, int b, const std::string& language);
std::string decryptAffineCipher(const std::string& text, int a, int b, const std::string& language);
int gcdAffine(int a, int b); // Добавляем свою функцию GCD
void runAffineCipher();

#endif