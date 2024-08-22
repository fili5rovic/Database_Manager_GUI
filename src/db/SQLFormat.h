#ifndef PRVIPROJEKAT_SQLFORMAT_H
#define PRVIPROJEKAT_SQLFORMAT_H

#include <fstream>
#include "Format.h"

class SQLFormat : public Format {
public:
    SQLFormat(shared_ptr<Database> database);

private:
    void printTableNames(std::stringstream &ss) const;

    void printDataInsertion(stringstream &ss) const override;

    string getFileExtension() const override;

    string getDir() const override;
};


#endif //PRVIPROJEKAT_SQLFORMAT_H
