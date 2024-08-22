#ifndef PRVIPROJEKAT_STRINGMANIPULATOR_H
#define PRVIPROJEKAT_STRINGMANIPULATOR_H

#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
#include "Table.h"

using namespace std;

class StringManipulator {
private:
    const char topLeftCorner = '\xDA';
    const char bottomLeftCorner = '\xC0';
    const char topRightCorner = '\xBF';
    const char bottomRightCorner = '\xD9';
    const char middleLeftCorner = '\xC3';
    const char middleRightCorner = '\xB4';
    const char horizontalLine = '\xC4';
    const char verticalLine = '\xB3';
    const char intersection = '\xC5';
    const char middleDown = '\xC2';
    const char middleUp = '\xC1';
    const string RED = "\033[38;2;255;0;0m";
    const string GREEN = "\033[32m";
    const string GRAY = "\033[37m";
    const string CYAN = "\033[36m";
    const string PURPLE = "\033[35m";
    const string RESET = "\033[0m";
    const string BOLD = "\033[1m";
public:

    static StringManipulator instance();

    string RGB(int r, int g, int b);

    const string &REDCOLOR() const;

    const string &GREENCOLOR() const;

    const string &GRAYCOLOR() const;
    const string &CYANCOLOR() const;
    const string &PURPLECOLOR() const;

    const string &RESETCOLOR() const;

    const string &BOLDCOLOR() const;

    string printline(int n);

    string printspace(int n);

    void upperTableHeading(int n, bool closing = true, bool end = true);

    void lowerTableHeading(int n, bool closing = true, bool end = true);

    void newMenu(int line_len, vector<string> options, int pomeraj = 7);

    void newMessage(string poruka, int line_len = -1);

    void newMessageGreen(string poruka, int line_len = -1);

    void newMessageRed(string poruka, int line_len = -1);

    static vector<string> splitString(const string &input, const char &delim = ' ');

    static void removeDuplicateSpaces(string &str);

    static void removeSpaces(string& str);


    string trim(const string& str);


    void removeQuotesAndLeadingSpaces(vector<string>& strings);



};


#endif //PRVIPROJEKAT_STRINGMANIPULATOR_H
