#ifndef PRVIPROJEKAT_TABLE_H
#define PRVIPROJEKAT_TABLE_H

#include <vector>
#include "Record.h"
#include "Errors.h"
#include <algorithm>
#include <regex>
#include <iomanip>
#include <numeric>

class Table {
public:
    Table(const string &name);

    Table(const Table &t);

    Table(Table &&t);

    Table &operator=(const Table &t);

    Table &operator=(Table &&t);

    friend ostream &operator<<(ostream &os, const Table &t);

    void tablePrintRaw(ostream& os) const;

    void addRecord(const Record &r);

    void addHeader(const string &s);

    Record& getRecordByIndex(int index);

    int getRecordIndex(const vector<string>& values) const;

    void removeRecordFromIndex(int index);


    int getColumnIndex(const string& columnName) const;

    //<editor-fold desc="Getters">

    // returns a table of a single column
    shared_ptr<Table> getSubTable(const string &colName) const;

    shared_ptr<Table> getSubTableByIndex(int columnIndex) const;



    static shared_ptr<Table> getMergedTable(const shared_ptr<Table> t1, const shared_ptr<Table> t2);

    static shared_ptr<Table> getMergedTablesForJoin(const shared_ptr<Table> t1, const shared_ptr<Table> t2);


    const string &getName() const;

    const vector<string> &getTableHeaders() const;

    const vector<Record> &getTableRecords() const;

    vector<Record>& getTableRecordsByReference();
    //</editor-fold>


private:
    string name;
    vector<Record> records;
    vector<string> header;


    void copy(const Table &t);

    void move(Table &t);


    void tablePrint(ostream &os) const;



    void upperPartTablePrint(vector<int> width, ostream &os) const;

    void middlePartTablePrint(vector<int> width,ostream &os, bool up = false) const;

    void bottomPartTablePrint(vector<int> width, ostream &os) const;

    vector<int> getWidthVectorForTablePrinting(int padding = 2) const;

};


#endif //PRVIPROJEKAT_TABLE_H
