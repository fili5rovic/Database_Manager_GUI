#include "Filter.h"

Filter::Filter(const shared_ptr<Table> t, const string &whereArgsString) {
    init(t, whereArgsString);
}

shared_ptr<Table> Filter::getFilteredTable() {
    return this->table;
}

void Filter::init(const shared_ptr<Table> t, const string &whereArgs) {
    if(whereArgs.empty()) {
        table = make_shared<Table>(*t);
        return;
    }
    setHeaderAndNameFromParameterTable(t);
    vector<string> orStrings = getOrStringsFromWhere(whereArgs);
    for (const string &conditionWithAnds: orStrings) {
        Table* tempTable = new Table(*t);
        vector<string> conditionStrings = getAndStringsFromOrStrings(conditionWithAnds);
        vector<shared_ptr<Condition>> innerVector;
        for (string &condStr: conditionStrings) {
            shared_ptr<Condition> condition;
            string valueStr;

            smatch matches;
            if (regex_search(condStr, matches,regex("^((?:\\w+\\.)?\\w+)\\s*\\=\\s*(?:(\\w+)|'(\\w+)'|\"(\\w+)\")$", regex_constants::icase))) {
                valueStr = getCorrectMatch(matches);
                condition = std::make_shared<Equal>(tempTable, matches[1], valueStr);
            } else if(regex_search(condStr, matches,regex("^((?:\\w+\\.)?\\w+)\\s*(?:\\!\\=|\\<\\>)\\s*(?:(\\w+)|'(\\w+)'|\"(\\w+)\")$", regex_constants::icase))) {
                valueStr = getCorrectMatch(matches);
                condition = std::make_shared<NotEqual>(tempTable, matches[1], valueStr);
            } else
                throw EBadArgumentsException("[SYNTAX_ERROR] Invalid WHERE syntax.");

            tempTable = condition->getConditionedTable();
            innerVector.push_back(condition);
        }
        this->conditions.push_back(innerVector);
    }

    updateTableRows();
}

void Filter::updateTableRows() const {
    Table* tempTable;
    for(const auto& vec : conditions) {
        // last vector has the result of all ANDs between an OR
        tempTable = vec.at(vec.size()-1)->getConditionedTable();
        for(Record& rec : tempTable->getTableRecordsByReference()) {
            table->addRecord(rec);
        }
    }
}
