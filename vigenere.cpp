#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>
#include "vigenere.h"
#include "common.h"

using namespace std;

// Проверяем, что символ находится в диапазоне ASCII (0-255)
inline bool isValidASCII(unsigned char c) {
    return true; // Принимаем все байты от 0 до 255
}

// Функция для расширения ключа до длины текста
string expandKey(const string& key, size_t length) {
    if (key.empty()) return "";

    string expandedKey;
    expandedKey.reserve(length);

    for (size_t i = 0; i < length; i++) {
        expandedKey += key[i % key.length()];
    }

    return expandedKey;
}

// Функция шифрования для всех ASCII символов
string encryptVigenereCipher(const string& text, const string& key) {
    if (key.empty()) {
        cerr << "Ошибка: ключ не может быть пустым" << endl;
        return "";
    }

    string expandedKey = expandKey(key, text.length());
    string result;
    result.reserve(text.length());

    int n = 256; // Размер алфавита ASCII

    for (size_t i = 0; i < text.length(); i++) {
        unsigned char textChar = static_cast<unsigned char>(text[i]);
        unsigned char keyChar = static_cast<unsigned char>(expandedKey[i]);

        if (isValidASCII(textChar)) {
            // Шифрование: (текст + ключ) mod 256
            int encrypted = (static_cast<int>(textChar) + static_cast<int>(keyChar)) % n;
            result += static_cast<unsigned char>(encrypted);
        }
        else {
            // На всякий случай оставляем символ как есть
            result += textChar;
        }
    }

    return result;
}

// Функция дешифрования для всех ASCII символов
string decryptVigenereCipher(const string& text, const string& key) {
    if (key.empty()) {
        cerr << "Ошибка: ключ не может быть пустым" << endl;
        return "";
    }

    string expandedKey = expandKey(key, text.length());
    string result;
    result.reserve(text.length());

    int n = 256; // Размер алфавита ASCII

    for (size_t i = 0; i < text.length(); i++) {
        unsigned char textChar = static_cast<unsigned char>(text[i]);
        unsigned char keyChar = static_cast<unsigned char>(expandedKey[i]);

        if (isValidASCII(textChar)) {
            // Дешифрование: (текст - ключ + 256) mod 256
            int decrypted = (static_cast<int>(textChar) - static_cast<int>(keyChar) + n) % n;
            result += static_cast<unsigned char>(decrypted);
        }
        else {
            // На всякий случай оставляем символ как есть
            result += textChar;
        }
    }

    return result;
}

// Функция для отображения байтов в читаемом формате
void displayBytesInfo(const string& text, const string& label) {
    cout << "\n" << label << " (" << text.length() << " байт):" << endl;

    // HEX представление (первые 50 байт)
    cout << "HEX: ";
    for (size_t i = 0; i < min(text.length(), size_t(50)); i++) {
        printf("%02X ", static_cast<unsigned char>(text[i]));
    }
    if (text.length() > 50) cout << "...";
    cout << endl;

    // Текстовое представление с поддержкой русских символов
    cout << "TEXT: ";
    for (size_t i = 0; i < min(text.length(), size_t(100)); i++) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        // Печатаемые ASCII + русские буквы Windows-1251
        if ((c >= 32 && c < 127) ||
            (c >= 192 && c <= 255 && c != 215 && c != 247)) { // Русские буквы в CP1251
            cout << c;
        }
        else if (c == '\n') {
            cout << "\\n";
        }
        else if (c == '\t') {
            cout << "\\t";
        }
        else if (c == '\r') {
            cout << "\\r";
        }
        else {
            cout << ".";
        }
    }
    if (text.length() > 100) cout << "...";
    cout << endl;

    // Дополнительная информация о составе
    int printable = 0, russian = 0, control = 0;
    for (unsigned char c : text) {
        if (c >= 32 && c < 127) printable++;
        else if (c >= 192 && c <= 255) russian++;
        else if (c < 32) control++;
    }
    cout << "Состав: " << printable << " печатных ASCII, "
        << russian << " русских/спец, "
        << control << " управляющих" << endl;
}

// Основная функция для работы с шифром Виженера
void runVigenereCipher() {
    string text, inputFilename, outputFilename, key;
    int action;
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
                cout << "Текст успешно прочитан из файла (" + to_string(text.length()) + " байт).\n";
                displayBytesInfo(text, "Исходные данные");
            }
            catch (const exception& e) {
                cerr << "Ошибка чтения файла: " << e.what() << endl;
                return;
            }
        }
    }
    else {
        cout << "Введите текст:\n";
        getline(cin, text);
        displayBytesInfo(text, "Введенный текст");
    }

    cout << "Введите ключевое слово или фразу: ";
    getline(cin, key);

    if (key.empty()) {
        cout << "Ошибка: ключ не может быть пустым\n";
        return;
    }

    cout << "Длина ключа: " << key.length() << " байт" << endl;
    displayBytesInfo(key, "Ключ");

    cout << "\nВыберите действие: 1 - Шифрование, 2 - Дешифрование\n";
    cin >> action;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string result;
    if (action == 1) {
        cout << "--- ШИФРОВАНИЕ ---" << endl;
        result = encryptVigenereCipher(text, key);
        if (!result.empty()) {
            displayBytesInfo(result, "Зашифрованный результат");
        }
    }
    else if (action == 2) {
        cout << "--- ДЕШИФРОВАНИЕ ---" << endl;
        result = decryptVigenereCipher(text, key);
        if (!result.empty()) {
            displayBytesInfo(result, "Расшифрованный результат");
        }
    }
    else {
        cout << "Ошибка: неверное действие\n";
        return;
    }

    if (result.empty()) {
        return; // Если была ошибка
    }

    // Всегда показываем результат в консоли
    cout << "\n=== ТЕКСТОВОЕ ПРЕДСТАВЛЕНИЕ ===" << endl;
    cout << "Результат:\n";

    // Показываем результат в читаемом виде
    size_t displayLimit = 1000;
    if (result.length() <= displayLimit) {
        cout << result << endl;
    }
    else {
        cout << result.substr(0, displayLimit) << "\n... [еще " << (result.length() - displayLimit) << " символов]\n";
    }

    cout << "\nСтатистика:" << endl;
    cout << "Общий размер: " << result.length() << " байт" << endl;

    cout << "\nСохранить в файл (y/n): ";
    cin >> useFile;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (useFile == 'y' || useFile == 'Y') {
        cout << "Введите имя выходного файла: ";
        getline(cin, outputFilename);

        if (writeToFile(outputFilename, result)) {
            cout << "Результат сохранен в файл: " << outputFilename << " (" << result.length() << " байт)" << endl;

            // Проверка сохранения для бинарных файлов
            try {
                string checkContent = readFile(outputFilename);
                if (checkContent == result) {
                    cout << "Файл успешно сохранен и проверен" << endl;
                }
                else {
                    cout << "Внимание: сохраненный файл отличается от ожидаемого" << endl;
                }
            }
            catch (const exception& e) {
                cerr << "Ошибка проверки файла: " << e.what() << endl;
            }
        }
    }
}