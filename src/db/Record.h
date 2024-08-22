#ifndef PRVIPROJEKAT_RECORD_H
#define PRVIPROJEKAT_RECORD_H

#include <iostream>
#include <vector>

using namespace std;

class Record {
public:
    Record();

    Record(const Record &r);

    Record(Record &&r);

    Record &operator=(const Record &r);

    Record &operator=(Record &&r);

    friend ostream &operator<<(ostream &os, const Record &r);

    void addData(const string &col);

    vector<string> getData() const;

    vector<string>& getDataReference();

private:
    vector<string> columns;

    void copy(const Record &r);

    void move(Record &r);


};


#endif //PRVIPROJEKAT_RECORD_H
