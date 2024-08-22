#ifndef PRVIPROJEKAT_DROP_H
#define PRVIPROJEKAT_DROP_H


#include "Statement.h"
#include "Database.h"

class Drop : public Statement {
public:
    Drop(const string &input, shared_ptr<Database> database);

private:
    void executingQuery(const smatch& matches) const override;

    //<editor-fold desc="Error Handling">
    void runtimeErrors(const string &input) const override;

    void checkForSyntaxErrors(const string &query) const override;
    //</editor-fold>

    //<editor-fold desc="Getters">
    regex getRegexPattern() const override;

    regex getRegexForFindingTable() const override;
    //</editor-fold>

};


#endif //PRVIPROJEKAT_DROP_H
