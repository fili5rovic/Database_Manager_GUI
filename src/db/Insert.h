#ifndef PRVIPROJEKAT_INSERT_H
#define PRVIPROJEKAT_INSERT_H

#include "Statement.h"
#include "Database.h"

class Insert : public Statement {
public:
    Insert(const string &input, shared_ptr<Database> database);

private:
    void executingQuery(const smatch &matches) const override;

    void sortColumnsAndValues(vector<string> &listOfColumns, vector<string> &values) const;

    // todo doesnt work if i don't write all of columns
    void checkIfAllColumnsAreThere(vector<string> listOfColumns, const size_t listOfValuesSize) const;


    //<editor-fold desc="Error Handling">
    void runtimeErrors(const string &input) const override;


    void checkForSyntaxErrors(const string &query) const override;
    //</editor-fold>


    //<editor-fold desc="Getters">
    regex getRegexPattern() const override;


    regex getRegexForFindingTable() const override;
    //</editor-fold>

};


#endif //PRVIPROJEKAT_INSERT_H
