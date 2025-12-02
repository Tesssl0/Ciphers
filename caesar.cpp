// caesar.cpp
#include <iostream>
#include <string>
#include <limits>
#include "caesar.h"
#include "common.h"
using namespace std;

bool validateTextCaesar(const string& text, const string& language) {
    return validateMixedText(text);
}

string encryptCaesarCipher(const string& text, int key, const string& language) {
    string result;
    int n = 256; // Работаем со всей ASCII таблицей

    for (unsigned char c : text) {
        int x = static_cast<unsigned char>(c);
        int y = (x + key) % n;
        if (y < 0) y += n;
        result += static_cast<unsigned char>(y);
    }
    return result;
}

string decryptCaesarCipher(const string& text, int key, const string& language) {
    return encryptCaesarCipher(text, -key, language);
}

void runCaesarCipher() {
    string text, inputFilename, outputFilename;
    int key, action;
    char useFile;

    cout << "=== Шифр Цезаря ===\n";
    cout << "Читать из файла? (y/n): ";
    cin >> useFile;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (useFile == 'y' || useFile == 'Y') {
        cout << "Введите имя входного файла: ";
        getline(cin, inputFilename);

        if (!fileExists(inputFilename)) {
            char createNew;
            cout << "Файл '" + inputFilename + "' не существует. Создать новый файл? (y/n): ";
            cin >> createNew;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (createNew == 'y' || createNew == 'Y') {
                string fileContent;
                cout << "Введите содержимое для нового файла: ";
                getline(cin, fileContent);

                if (!createFile(inputFilename, fileContent)) {
                    return;
                }
                text = fileContent;
            }
            else {
                return;
            }
        }
        else {
            try {
                text = readFile(inputFilename);
                cout << "Текст успешно прочитан из файла (" + to_string(text.length()) + " символов).\n";
            }
            catch (const exception& e) {
                cerr << "Ошибка чтения файла: " << e.what() << endl;
                return;
            }
        }
    }
    else {
        cout << "Введите текст (можно использовать любые символы ASCII):\n";
        getline(cin, text);
    }

    if (!validateMixedText(text)) {
        cout << "Ошибка: текст содержит недопустимые символы.\n";
        return;
    }

    cout << "Выберите действие: 1 - Шифрование, 2 - Дешифрование\n";
    cin >> action;

    cout << "Введите ключ (число): ";
    cin >> key;

    string result;
    if (action == 1) {
        result = encryptCaesarCipher(text, key, "");
        cout << "Зашифрованный текст: ";
    }
    else {
        result = decryptCaesarCipher(text, key, "");
        cout << "Расшифрованный текст: ";
    }

    cout << result << endl;

    cout << "Сохранить в файл? (y/n): ";
    cin >> useFile;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (useFile == 'y' || useFile == 'Y') {
        cout << "Введите имя выходного файла: ";
        getline(cin, outputFilename);

        if (writeToFile(outputFilename, result)) {
            cout << "Результат также сохранен в файл: " << outputFilename << endl;
        }
    }
}