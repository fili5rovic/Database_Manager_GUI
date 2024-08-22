#ifndef PRVIPROJEKAT_CREATE_H
#define PRVIPROJEKAT_CREATE_H


#include "Statement.h"
#include "StringManipulator.h"
#include "Database.h"

class Create : public Statement {
public:
    Create(const string &input, shared_ptr<Database> database);


    void execute() override;


private:

    void executingQuery(const smatch &matches) const override;

    string getTableNameFromCreateQuery(const string &query) const;


    //<editor-fold desc="Error Handling">
    void runtimeErrors(const string &input) const override;


    void checkForSyntaxErrors(const string &query) const override;
    //</editor-fold>


    //<editor-fold desc="Getters">
    regex getRegexPattern() const override;

    regex getRegexForFindingTable() const override;
    //</editor-fold>

};


#endif //PRVIPROJEKAT_CREATE_H
