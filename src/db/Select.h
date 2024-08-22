#ifndef PRVIPROJEKAT_SELECT_H
#define PRVIPROJEKAT_SELECT_H

#include <regex>
#include "Statement.h"
#include "Filter.h"
#include "Errors.h"
#include "StringManipulator.h"
#include "Database.h"


class Select : public Statement {
public:
    Select(const string &input, shared_ptr<Database> database);

private:
    void executingQuery(const smatch &matches) const override;

    void join(const string& argumentsStr, string &fromTableStr, string whereStr) const;

    vector<int> getVectorOfStartPositionOfEachTable(const map<string,shared_ptr<Table>>& tableNickNames) const;


    void checkForAmbiguity(const string &column,const shared_ptr<Table>& table) const;

    shared_ptr<Table> getTableWithSelectedColumnsJoin(shared_ptr<Table> tableArg,vector<string> selectedColumns, map<string, shared_ptr<Table>> tableNicknames) const;

    static int getIndexOfAcronymInMap(const map<string, shared_ptr<Table>> tableNicknames, const string& acronym);

    static shared_ptr<Table> getTableWithSelectedColumns(shared_ptr<Table> tableArg,vector<string> selectedColumns);

    static vector<string> getProcessedSelectedColumns(shared_ptr<Table> tableArg, vector<string> selectedColumns);

    //<editor-fold desc="Error Handling">
    void runtimeErrors(const std::string &input) const override;

    void checkForSyntaxErrors(const string &query) const;
    //</editor-fold>

    static std::string toLower(const std::string &str);

    //<editor-fold desc="Getters">
    regex getRegexPattern() const override;

    regex getRegexForFindingTable() const override;
    //</editor-fold>


};


#endif //PRVIPROJEKAT_SELECT_H
