#ifndef PRVIPROJEKAT_DATABASE_H
#define PRVIPROJEKAT_DATABASE_H

#include <iostream>
#include <unordered_map>
#include "Table.h"
#include "StringManipulator.h"

using namespace std;

class Database {
public:
    Database(const string &name);

    Database(const Database &d) = delete;

    Database(Database &&d) = delete;

    Database &operator=(const Database &d) = delete;

    Database &operator=(Database &&d) = delete;

    const string &getName() const;

    void addTable(const Table &t);

    void removeTable(const string &name);

    friend ostream &operator<<(ostream &os, const Database &d);

    shared_ptr<Table> tryGettingTableByNameCaseI(const string &tableName) const;

    vector<string> getAllTableNames() const;

    vector<string> getAllHeaders() const;

    const unordered_map<string, shared_ptr<Table>>& getTablePairs() const;

    ////

    void printDatabaseRaw(ostream &os) const;

    ////

private:
    const string name;
    unordered_map<string, shared_ptr<Table>> tables;
};


#endif //PRVIPROJEKAT_DATABASE_H
