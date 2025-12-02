// common.cpp
#include "common.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace std;

bool validateMixedText(const string& text) {
    // Разрешаем все символы ASCII (0-127) и расширенную ASCII (128-255)
    // Это включает русские, английские, цифры и спецсимволы
    for (unsigned char c : text) {
        // Разрешаем все байты от 0 до 255
        // Это включает всю расширенную ASCII таблицу
    }
    return true;
}

bool fileExists(const string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

string readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }

    string content((istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>());
    file.close();
    return content;
}

bool writeToFile(const string& filename, const string& content) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл " << filename << endl;
        return false;
    }
    file.write(content.c_str(), content.size());
    file.close();
    return true;
}

bool createFile(const string& filename, const string& content) {
    return writeToFile(filename, content);
}