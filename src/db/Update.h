#ifndef PRVIPROJEKAT_UPDATE_H
#define PRVIPROJEKAT_UPDATE_H


#include "Statement.h"
#include "Filter.h"

class Update : public Statement {
public:
    Update(const string &input, shared_ptr<Database> database);

private:

    void executingQuery(const std::smatch &matches) const override;

    void printAffectedRowsMessage(int num) const;

    //<editor-fold desc="Error Handling">
    void runtimeErrors(const std::string &input) const override;

    void checkForSyntaxErrors(const std::string &query) const override;
    //</editor-fold>

    //<editor-fold desc="Getters">
    regex getRegexForFindingTable() const override;


    regex getRegexPattern() const override;
    //</editor-fold>
};


#endif //PRVIPROJEKAT_UPDATE_H
