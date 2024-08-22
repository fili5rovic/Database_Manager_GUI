#ifndef PRVIPROJEKAT_STATEMENT_H
#define PRVIPROJEKAT_STATEMENT_H


#include "Table.h"
#include "Database.h"

class Statement {
public:

    virtual void execute() {
        table = tryToGetTableFromQuery(inputQuery, database, getRegexForFindingTable());
        handleQuery(inputQuery);
    }

protected:
    shared_ptr<Table> table;
    const string inputQuery;
    shared_ptr<Database> database;

    Statement(const string &inputQuery, shared_ptr<Database> database) : database(database), inputQuery(inputQuery), table(nullptr) {}

    void handleQuery(const string &input) const {
        smatch matches;
        if (regex_match(input, matches, getRegexPattern()))
            executingQuery(matches);
        else
            runtimeErrors(input);
    }

    virtual void executingQuery(const smatch &matches) const = 0;

    virtual regex getRegexPattern() const = 0;

    virtual regex getRegexForFindingTable() const = 0;

    virtual void runtimeErrors(const string &input) const = 0;

    virtual void checkForSyntaxErrors(const string &query) const {};

private:
    shared_ptr<Table> tryToGetTableFromQuery(const string &query, const shared_ptr<Database> database, const regex &regexPattern) const {
        checkForSyntaxErrors(query);
        checkForMismatchedQuotes(query);

        shared_ptr<Table> currTable = nullptr;

        std::smatch matches;
        if (regex_search(query, matches, regexPattern)) { // here
            if (matches.size() > 1) {
                currTable = database->tryGettingTableByNameCaseI(matches[1]);
            }
        }

        if (!currTable) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Table not found.");
        }
        return currTable;
    }

    void checkForMismatchedQuotes(const string &query) const {
        int count = 0;
        for (char i: query) {
            if (i == '\'') {
                count++;
            }
        }
        if (count % 2 != 0) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Mismatched quotes.");
        }
        count = 0;
        for (char i: query) {
            if (i == '\"') {
                count++;
            }
        }
        if (count % 2 != 0) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Mismatched double quotes.");
        }
        count = 0;
        for (char i: query) {
            if (i == '(') {
                count++;
            }
            else if(i == ')') {
                count--;
            }
        }
        if (count != 0) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Mismatched parentheses.");
        }
    }

};


#endif //PRVIPROJEKAT_STATEMENT_H