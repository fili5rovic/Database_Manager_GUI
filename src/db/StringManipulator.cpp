#include "StringManipulator.h"

StringManipulator StringManipulator::instance() {
    static StringManipulator instance;
    return instance;
}

string StringManipulator::RGB(int r, int g, int b) {
    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

const string &StringManipulator::REDCOLOR() const {
    return RED;
}

const string &StringManipulator::GREENCOLOR() const {
    return GREEN;
}

const string &StringManipulator::GRAYCOLOR() const {
    return GRAY;
}

const string &StringManipulator::CYANCOLOR() const {
    return CYAN;
}

const string &StringManipulator::PURPLECOLOR() const {
    return PURPLE;
}

const string &StringManipulator::RESETCOLOR() const {
    return RESET;
}

const string &StringManipulator::BOLDCOLOR() const {
    return BOLD;
}

string StringManipulator::printline(int n) {
    string s;
    for (int i = 0; i < n; ++i) {
        s += '\xC4';
    }
    return s;
}

string StringManipulator::printspace(int n) {
    string s;
    for (int i = 0; i < n; ++i) {
        s += ' ';
    }
    return s;
}

void StringManipulator::upperTableHeading(int n, bool closing, bool end) {
    cout << topLeftCorner << printline(n);
    if (closing) cout << topRightCorner;
    if (end) cout << endl;
}

void StringManipulator::lowerTableHeading(int n, bool closing, bool end) {
    cout << bottomLeftCorner << printline(n);
    if (closing) cout << bottomRightCorner;
    if (end) cout << endl;
}

void StringManipulator::newMenu(int line_len, vector<string> options, int pomeraj) {
    for (int i = 0; i < options.size(); ++i) {
        if (options[i].length() % 2 != 0)
            options[i] += " ";
    }

    cout << endl;
    upperTableHeading(line_len);
    cout << verticalLine << printspace((line_len - options[0].length()) / 2) << options[0]
         << printspace((line_len - options[0].length()) / 2) << verticalLine << endl;
    cout << middleLeftCorner << printline(line_len) << middleRightCorner << endl;
    for (int i = 1; i < options.size(); ++i) {
        cout << verticalLine << printspace(pomeraj) << options[i]
             << printspace((line_len - options[i].length()) - pomeraj) << verticalLine << endl;
        if (i < options.size() - 1)
            cout << middleLeftCorner << printline(line_len) << middleRightCorner << endl;
    }
    lowerTableHeading(line_len);
}

void StringManipulator::newMessage(string poruka, int line_len) {
    if (line_len == -1)
        line_len = poruka.length() + 6;
    else if (poruka.length() % 2 != 0)
        poruka += " ";
    cout << endl;
    upperTableHeading(line_len);
    cout << verticalLine << printspace((line_len - poruka.length()) / 2) << poruka
         << printspace((line_len - poruka.length()) / 2) << verticalLine << endl;
    lowerTableHeading(line_len);
}

void StringManipulator::newMessageGreen(string poruka, int line_len) {
    cout << GREEN;
    newMessage(std::move(poruka), line_len);
    cout << RESET;
}

void StringManipulator::newMessageRed(string poruka, int line_len) {
    cout << RED;
    newMessage(std::move(poruka), line_len);
    cout << RESET;
}

vector<string> StringManipulator::splitString(const string &input, const char &delim) {
    vector<string> tokens;
    istringstream iss(input);
    string token;
    while (getline(iss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

void StringManipulator::removeDuplicateSpaces(string &str) {
    str.erase(std::unique(str.begin(), str.end(), [](char a, char b) {
        return a == ' ' && b == ' ';
    }), str.end());
}

void StringManipulator::removeSpaces(string &str) {
    string result;
    for (char c : str) {
        if (c != ' ') {
            result += c;
        }
    }
    str = result;
}

string StringManipulator::trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos)
        return "";

    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

void StringManipulator::removeQuotesAndLeadingSpaces(vector<string> &strings) {
    vector<string> result;
    for (string& str : strings) {
        str = trim(str);
        if ((str.front() == '\"' && str.back() == '\"') || (str.front() == '\'' && str.back() == '\''))
            str = str.substr(1, str.size() - 2);
        result.push_back(str);
    }
}
