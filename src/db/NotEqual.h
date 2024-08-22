#ifndef PRVIPROJEKAT_NOTEQUAL_H
#define PRVIPROJEKAT_NOTEQUAL_H

#include "Condition.h"

class NotEqual : public Condition {
public:
    NotEqual(Table* table, const string &columnName, const string &value);

    bool evaluate(const Record &record) const override;
private:
};


#endif //PRVIPROJEKAT_NOTEQUAL_H
