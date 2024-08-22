#ifndef PRVIPROJEKAT_CUSTOMFORMAT_H
#define PRVIPROJEKAT_CUSTOMFORMAT_H

#include "Format.h"

class CustomFormat : public Format {
public:
    CustomFormat(const shared_ptr<Database> database);

    static shared_ptr<Database> createDatabaseFromFile(const string& dirPath, const string& dbName);

private:
    void printTableNames(stringstream &ss) const override;

    void printDataInsertion(stringstream &ss) const override;

    string getFileExtension() const override;

    string getDir() const override;


};


#endif //PRVIPROJEKAT_CUSTOMFORMAT_H
