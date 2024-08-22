//
// Created by fili5 on 19.4.2024..
//

#include "Record.h"

Record::Record() : columns() {}

Record::Record(const Record &r) {
    copy(r);
}

Record::Record(Record &&r) {
    move(r);
}

Record &Record::operator=(const Record &r) {
    copy(r);
    return *this;
}

Record &Record::operator=(Record &&r) {
    move(r);
    return *this;
}

ostream &operator<<(ostream &os, const Record &r) {
    for (auto it = r.columns.begin(); it != r.columns.end(); ++it) {
        if (it != r.columns.begin()) {
            os << ' '; // Add space between columns, but not before the first column
        }
        os << *it; // Output the current column
    }
    os << endl;
    return os;
}

void Record::addData(const string &col) {
    this->columns.push_back(col);
}

vector<string> Record::getData() const {
    return this->columns;
}

vector<string> &Record::getDataReference() {
    return this->columns;
}

void Record::copy(const Record &r) {
    this->columns = vector<string>();
    for (const string &s: r.columns) {
        this->columns.push_back(s);
    }
}

void Record::move(Record &r) {
    this->columns = r.columns;
    r.columns = vector<string>();
}
