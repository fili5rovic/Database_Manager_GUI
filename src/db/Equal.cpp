//
// Created by fili5 on 19.4.2024..
//

#include "Equal.h"

Equal::Equal(Table *table, const string &columnName, const string &value) : Condition(table, columnName, value) {}

bool Equal::evaluate(const Record &record) const {
    return record.getData().at(this->fieldPositionInVector) == this->value;
}
