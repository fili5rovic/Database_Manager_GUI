#include "Database.h"


//////




//////


const string &Database::getName() const {
    return name;
}

Database::Database(const string &name) : name(name), tables() {}

void Database::addTable(const Table &t) {
    tables.insert({t.getName(), make_shared<Table>(t)});
}

void Database::removeTable(const string &name) {
    if (tables.find(name) == tables.end())
        return;
    tables.erase(name);
}

ostream &operator<<(ostream &os, const Database &d) {
    os << endl << "Database \033[35m" << d.name << "\033[0m:" << endl;
    os << "---------------------------------------" << endl;
    for (auto &table: d.tables) {
        os << *table.second << endl;
    }
    os << "---------------------------------------" << endl;
    return os;
}

shared_ptr<Table> Database::tryGettingTableByNameCaseI(const string &tableName) const {
    for (const auto &pair: tables) {
        if (regex_match(pair.first, regex(tableName, regex_constants::icase)))
            return pair.second;
    }
    return nullptr;
}

vector<string> Database::getAllTableNames() const {
    vector<string> retVector;
    retVector.reserve(tables.size());
    for (const auto &pair: tables) {
        retVector.push_back(pair.first);
    }
    return retVector;
}

vector<string> Database::getAllHeaders() const {
    vector<string> retVector;
    for (const auto &header: tables) {
        for (const auto &str: header.second->getTableHeaders()) {
            retVector.push_back(str);
        }
    }
    return retVector;
}

const unordered_map<string, shared_ptr<Table>> &Database::getTablePairs() const {
    return tables;
}

void Database::printDatabaseRaw(ostream &os) const {
    for (const auto &table: tables) {
        table.second->tablePrintRaw(os);
        os << "@" << endl;
    }
}
