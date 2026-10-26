#include <iostream>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

const string USER_FILE = "users.txt";
const int SALT_SIZE = 16;
const int HASH_SIZE = 32;
const int PBKDF2_ITERATIONS = 200000;

string toHex(const unsigned char* data, int length) {
    stringstream ss;
    ss << hex << setfill('0');
    for (int i = 0; i < length; ++i) {
        ss << setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

bool fromHex(const string& hexText, unsigned char* output, int expectedLength) {
    if (hexText.size() != static_cast<size_t>(expectedLength * 2)) return false;
    for (int i = 0; i < expectedLength; ++i) {
        try {
            output[i] = static_cast<unsigned char>(
                stoi(hexText.substr(i * 2, 2), nullptr, 16));
        } catch (...) {
            return false;
        }
    }
    return true;
}

bool makePasswordHash(const string& password, string& saltHex, string& hashHex) {
    unsigned char salt[SALT_SIZE];
    unsigned char hash[HASH_SIZE];

    if (RAND_bytes(salt, SALT_SIZE) != 1) return false;

    if (PKCS5_PBKDF2_HMAC(
            password.c_str(), static_cast<int>(password.size()),
            salt, SALT_SIZE, PBKDF2_ITERATIONS,
            EVP_sha256(), HASH_SIZE, hash) != 1) {
        return false;
    }

    saltHex = toHex(salt, SALT_SIZE);
    hashHex = toHex(hash, HASH_SIZE);
    return true;
}

bool verifyPassword(const string& password, const string& saltHex,
                    const string& storedHashHex) {
    unsigned char salt[SALT_SIZE];
    unsigned char storedHash[HASH_SIZE];
    unsigned char candidateHash[HASH_SIZE];

    if (!fromHex(saltHex, salt, SALT_SIZE) ||
        !fromHex(storedHashHex, storedHash, HASH_SIZE)) {
        return false;
    }

    if (PKCS5_PBKDF2_HMAC(
            password.c_str(), static_cast<int>(password.size()),
            salt, SALT_SIZE, PBKDF2_ITERATIONS,
            EVP_sha256(), HASH_SIZE, candidateHash) != 1) {
        return false;
    }

    // Compare all bytes rather than returning early.
    unsigned char difference = 0;
    for (int i = 0; i < HASH_SIZE; ++i) {
        difference |= candidateHash[i] ^ storedHash[i];
    }
    return difference == 0;
}

bool usernameExists(const string& username) {
    ifstream file(USER_FILE);
    string savedUsername, salt, hash;
    while (file >> savedUsername >> salt >> hash) {
        if (savedUsername == username) return true;
    }
    return false;
}

bool validUsername(const string& username) {
    if (username.size() < 3 || username.size() > 20) return false;
    for (unsigned char c : username) {
        if (!(isalnum(c) || c == '_' || c == '-')) return false;
    }
    return true;
}

void registerUser() {
    string username, password;
    cout << "Choose username (3-20 letters/numbers/_/-): ";
    cin >> username;

    if (!validUsername(username)) {
        cout << "Error: Invalid username format.\n";
        return;
    }

    if (usernameExists(username)) {
        cout << "Error: Username already exists. Please choose another.\n";
        return;
    }

    cout << "Choose password (at least 8 characters): ";
    cin >> password;
    if (password.size() < 8) {
        cout << "Error: Password must be at least 8 characters.\n";
        return;
    }

    string saltHex, hashHex;
    if (!makePasswordHash(password, saltHex, hashHex)) {
        cout << "Error: Could not securely process password.\n";
        return;
    }

    ofstream file(USER_FILE, ios::app);
    if (!file) {
        cout << "Error: Could not open user database file.\n";
        return;
    }
    file << username << ' ' << saltHex << ' ' << hashHex << '\n';
    if (!file) {
        cout << "Error: Could not save registration.\n";
        return;
    }

    cout << "Registration successful!\n";
}

void loginUser() {
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    ifstream file(USER_FILE);
    if (!file) {
        cout << "Error: No registered users found. Please register first.\n";
        return;
    }

    string savedUsername, saltHex, hashHex;
    while (file >> savedUsername >> saltHex >> hashHex) {
        if (savedUsername == username) {
            if (verifyPassword(password, saltHex, hashHex)) {
                cout << "Login successful. Welcome, " << username << "!\n";
            } else {
                cout << "Error: Incorrect username or password.\n";
            }
            return;
        }
    }

    cout << "Error: Incorrect username or password.\n";
}

int main() {
    int choice;

    while (true) {
        cout << "\n=== Login and Registration System ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice. Please select 1, 2, or 3.\n";
        }
    }
}
