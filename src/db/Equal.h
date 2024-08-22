#ifndef PRVIPROJEKAT_EQUAL_H
#define PRVIPROJEKAT_EQUAL_H


#include "Condition.h"

class Equal : public Condition {
public:
    Equal(Table* table, const string &columnName, const string &value);

    bool evaluate(const Record &record) const override;
};


#endif //PRVIPROJEKAT_EQUAL_H
