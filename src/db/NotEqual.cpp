//
// Created by fili5 on 19.4.2024..
//

#include "NotEqual.h"

NotEqual::NotEqual(Table *table, const string &columnName, const string &value) : Condition(table, columnName, value) {}

bool NotEqual::evaluate(const Record &record) const {
    return record.getData().at(this->fieldPositionInVector) != this->value;
}
