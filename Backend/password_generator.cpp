#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>

using namespace std;

enum Strength { WEAK, MEDIUM, STRONG };

// Pick random character
char getRandomChar(const string& charset) {
    return charset[rand() % charset.length()];
}

// Calculate password strength
Strength getPasswordStrength(const string& password) {
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;

    for (char c : password) {
        if (islower(c)) hasLower = true;
        else if (isupper(c)) hasUpper = true;
        else if (isdigit(c)) hasDigit = true;
        else if (ispunct(c)) hasSpecial = true;
    }

    int score = hasLower + hasUpper + hasDigit + hasSpecial;

    if (password.length() >= 12 && score >= 3) return STRONG;
    if (password.length() >= 8 && score >= 2) return MEDIUM;
    return WEAK;
}

// Convert strength to text
string strengthToText(Strength s) {
    if (s == STRONG) return "Password Strength: Strong";
    if (s == MEDIUM) return "Password Strength: Medium";
    return "Password Strength: Weak";
}

// Generate password
string generatePassword(int length,
                        bool includeLower,
                        bool includeUpper,
                        bool includeDigits,
                        bool includeSpecial) {

    string charset = "";

    if (includeLower)
        charset += "abcdefghijklmnopqrstuvwxyz";
    if (includeUpper)
        charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (includeDigits)
        charset += "0123456789";
    if (includeSpecial)
        charset += "!@#$%^&*()-_=+[]{}|;:,.<>?/";

    if (charset.empty())
        return "";

    string password = "";
    for (int i = 0; i < length; i++) {
        password += getRandomChar(charset);
    }

    return password;
}

int main() {
    srand(time(0));

    // ---------- READ QUERY STRING ----------
    string query = "";
    char* qs = getenv("QUERY_STRING");
    if (qs != nullptr)
        query = qs;

    // ---------- DEFAULT VALUES ----------
    int length = 8;
    bool lower = false, upper = false, digits = false, special = false;

    // ---------- PARSE PARAMETERS ----------
    size_t pos;

    pos = query.find("length=");
    if (pos != string::npos)
        length = stoi(query.substr(pos + 7));

    if (query.find("lower=1")   != string::npos) lower = true;
    if (query.find("upper=1")   != string::npos) upper = true;
    if (query.find("digits=1")  != string::npos) digits = true;
    if (query.find("special=1") != string::npos) special = true;

    // ---------- CGI HEADER ----------
    cout << "Content-type:text/plain\n\n";

    // ---------- VALIDATION ----------
    if (!lower && !upper && !digits && !special) {
        cout << "ERROR\n";
        cout << "Select at least one character type";
        return 0;
    }

    if (length < 4) {
        cout << "ERROR\n";
        cout << "Password length must be at least 4";
        return 0;
    }

    // ---------- GENERATE PASSWORD ----------
    string password = generatePassword(length, lower, upper, digits, special);
    Strength strength = getPasswordStrength(password);

    // ---------- OUTPUT (FIXED FORMAT) ----------
    cout << "Generated Password: " << password << "\n";
    cout << strengthToText(strength);

    return 0;
}
