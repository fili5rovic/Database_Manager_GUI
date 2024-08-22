#ifndef PRVIPROJEKAT_DELETE_H
#define PRVIPROJEKAT_DELETE_H


#include "Statement.h"
#include "Filter.h"

class Delete : public Statement {
public:
    Delete(const string &input, shared_ptr<Database> database);
private:
    virtual void executingQuery(const smatch &matches) const;

    void runtimeErrors(const string &input) const override;

    void checkForSyntaxErrors(const string &query) const override;


    regex getRegexPattern() const override;

    regex getRegexForFindingTable() const override;
};


#endif //PRVIPROJEKAT_DELETE_H
