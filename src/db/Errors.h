#ifndef PRVIPROJEKAT_ERRORS_H
#define PRVIPROJEKAT_ERRORS_H

#include <iostream>
#include <fstream>

using namespace std;

class MyException : public exception {
public:
    MyException(const string msg) : msg(msg) {
        this->msg = colorBrackets(msg);
        writeToErrorFile(msg);
    }

    void writeToErrorFile(const string& message) const {
        ofstream errorFile("error.txt", ios::out);
        errorFile << message << endl;
        errorFile.close();

    }

    virtual const char * what() const noexcept override {
        return msg.c_str();
    }

    string colorBrackets(const std::string& str) const {
        string result = "\033[1m";
        for(const char& c : str) {
            if(c == '[') {
                result += "\033[38;2;255;0;0m";
            } else if(c == ']') {
                result += ']';
                result += "\033[0m\033[1m";
                continue;
            }
            result += c;
        }
        result += "\033[0m";
        return result;
    }

private:
    string msg;
};

class SyntaxError : public MyException {
public:
    SyntaxError(const string msg) : MyException(msg) {}
};

class RuntimeError : public MyException {
public:
    RuntimeError(const string msg) : MyException(msg) {}
};

class EMultipleKeywordsException : public SyntaxError {
public:
    EMultipleKeywordsException(const string msg) : SyntaxError(msg) {}
};

class EMissingKeywordsException : public SyntaxError {
public:
    EMissingKeywordsException(const string msg) : SyntaxError(msg) {}
};

class EMissingArgumentsException : public SyntaxError {
public:
    EMissingArgumentsException(const string msg) : SyntaxError(msg) {}
};

class EBadArgumentsException : public RuntimeError {
public:
    EBadArgumentsException(const string msg) : RuntimeError(msg) {}
};

class ENoKeywordsException : public SyntaxError {
public:
    ENoKeywordsException(const string msg) : SyntaxError(msg) {}
};


class EInvalidColumnNameException : public RuntimeError {
public:
    EInvalidColumnNameException(const string msg) : RuntimeError(msg) {}
};

class EQuotationsNotClosedException : public MyException {
public:
    EQuotationsNotClosedException(const string msg) : MyException(msg) {}
};


class EFileNotOpen : public MyException {
public:
    EFileNotOpen(const string msg) : MyException(msg) {}
};


#endif //PRVIPROJEKAT_ERRORS_H
