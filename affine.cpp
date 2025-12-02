#include <iostream>
#include <string>
#include <limits>
#include "affine.h"
#include "common.h"
using namespace std;

// Проверяем, что символ находится в диапазоне ASCII (0-255)
inline bool isValidASCII(unsigned char c) {
    return true; // Принимаем все байты от 0 до 255
}

bool validateTextAffine(const string& text, const string& language) {
    return validateMixedText(text);
}

int gcdAffine(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modInverseAffine(int a, int n) {
    a = a % n;
    for (int x = 1; x < n; x++) {
        if ((a * x) % n == 1)
            return x;
    }
    return -1;
}

string encryptAffineCipher(const string& text, int a, int b, const string& language) {
    string result;
    int n = 256; // Работаем со всей ASCII таблицей (256 символов)

    for (unsigned char c : text) {
        if (isValidASCII(c)) {
            int x = static_cast<unsigned char>(c);
            int y = (a * x + b) % n;
            if (y < 0) y += n;
            result += static_cast<unsigned char>(y);
        }
        else {
            result += c; // На всякий случай оставляем символ как есть
        }
    }
    return result;
}

string decryptAffineCipher(const string& text, int a, int b, const string& language) {
    string result;
    int n = 256; // Работаем со всей ASCII таблицей (256 символов)
    int a_inv = modInverseAffine(a, n);

    if (a_inv == -1) {
        cerr << "Ошибка: ключ 'a' не имеет обратного элемента по модулю " << n << endl;
        return "";
    }

    for (unsigned char c : text) {
        if (isValidASCII(c)) {
            int y = static_cast<unsigned char>(c);
            int x = (a_inv * (y - b + n)) % n;
            if (x < 0) x += n;
            result += static_cast<unsigned char>(x);
        }
        else {
            result += c; // На всякий случай оставляем символ как есть
        }
    }
    return result;
}

void runAffineCipher() {
    string text, inputFilename, outputFilename;
    int a, b, action;
    char useFile;

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

    cout << "Введите ключи a и b (через пробел):\n";
    cin >> a >> b;

    // Проверяем НОД для модуля 256
    if (gcdAffine(a, 256) != 1) {
        cout << "Ошибка: ключ 'a' должен быть взаимно простым с 256\n";
        return;
    }

    string result = (action == 1) ? encryptAffineCipher(text, a, b, "")
        : decryptAffineCipher(text, a, b, "");

    if (result.empty()) {
        return; // Если была ошибка в дешифровании
    }

    // Всегда показываем результат в консоли
    cout << "Результат: " << result << endl;

    cout << "Сохранить в файл(y/n): ";
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