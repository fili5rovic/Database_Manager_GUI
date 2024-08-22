#include "Select.h"

//const regex Select::regexPattern = regex(R"(^\s*select\s+((?:\w+|\*)(\s*,\s*(?:\w+|\*))*)\s+from\s+(\w+)+)", regex_constants::icase);

Select::Select(const string &input, shared_ptr<Database> database) : Statement(input, database) {}

void Select::executingQuery(const smatch &matches) const {
    shared_ptr<Table> selectTable = this->table;

    string argumentsStr = matches[1];
    StringManipulator::removeSpaces(argumentsStr);
    string fromTableStr = matches[2];
    string whereStr = matches[3];

    std::smatch matchForElse;
    if (regex_search(fromTableStr,
                     regex("\\w+(?:\\s+\\w+)?((?:\\s+inner)?\\s+join\\s+\\w+(?:\\s+\\w+)?\\s+on\\s+\\w+\\.\\w+\\s*=\\s*\\w+\\.\\w+)+",
                           regex_constants::icase))) {
        join(argumentsStr,fromTableStr, whereStr);
        return;
    } else if (regex_match(fromTableStr, matchForElse,
                           regex("^\\s*(\\w+)(?:\\s+(\\w+))\\s*$", regex_constants::icase))) {}

    vector<string> selectedColumns = StringManipulator::splitString(argumentsStr, ',');

    Filter f(selectTable, whereStr);
    shared_ptr<Table> filteredTable = f.getFilteredTable();

    // obrisi error fajl
    ofstream errorFile("error.txt", ios::out);
    errorFile << "";
    errorFile.close();


    // upisi u ovaj fajl isto
    ofstream outFile("output.txt", ios::out);
    getTableWithSelectedColumns(filteredTable, selectedColumns)->tablePrintRaw(outFile);
    outFile.close();


    cout << *getTableWithSelectedColumns(filteredTable, selectedColumns);
}

void Select::join(const string &argumentsStr, string &fromTableStr, string whereStr) const {
    fromTableStr = regex_replace(fromTableStr, regex("\\s*INNER\\s*", regex_constants::icase), " ");
    fromTableStr = regex_replace(fromTableStr, regex("\\s*JOIN\\s*", regex_constants::icase), "|");

    vector<string> subStrings = StringManipulator::instance().splitString(fromTableStr, '|');

    map<string, shared_ptr<Table>> tableNickNames;
    std::smatch innerMatches;
    if (regex_match(subStrings[0], innerMatches, regex("(\\w+)(?:\\s+(\\w+))?", regex_constants::icase))) {
        string tableName = innerMatches[1];
        string tableNick = innerMatches[2];

        cout << "Table name:" << tableName << endl;
        cout << "Table nick:" << tableNick << endl;

        if (database->tryGettingTableByNameCaseI(tableName) == nullptr) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Table '" + tableName + "' not found.");
        }

        tableNickNames[tableNick] = database->tryGettingTableByNameCaseI(tableName);

    } else throw EBadArgumentsException("[ERROR] Bad Syntax around: " + subStrings[0]);

    vector<shared_ptr<Table>> mergedTables;

    shared_ptr<Table> finalTable;

    for (auto it = subStrings.begin() + 1; it != subStrings.end(); ++it) {
        if (regex_search(*it, innerMatches,
                         regex("(\\w+)(?:\\s+(\\w+))?\\s+on\\s+(\\w+)\\.(\\w+)\\s*(\\=|\\!\\=)\\s*(\\w+)\\.(\\w+)",
                               regex_constants::icase))) {
            string tableName = innerMatches[1];
            string tableAcronym = innerMatches[2];
            string firstAcronym = innerMatches[3];
            string firstTableColumnNameInJoin = innerMatches[4];
            string equasion = innerMatches[5];
            string secondAcronym = innerMatches[6];
            string secondTableColumnNameInJoin = innerMatches[7];
//            cout << "Table name:" << tableName << endl;
//            cout << "Table acronym:" << tableAcronym << endl;
//            cout << "First acronym:" << firstAcronym << endl;
//            cout << "First table column:" << firstTableColumnNameInJoin << endl;
//            cout << "Equasion:" << equasion << endl;
//            cout << "Second acronym:" << secondAcronym << endl;
//            cout << "Second table column:" << secondTableColumnNameInJoin << endl;

            if (database->tryGettingTableByNameCaseI(tableName) != nullptr) {
                tableNickNames[tableAcronym] = database->tryGettingTableByNameCaseI(tableName);
            }else if (!tableAcronym.empty()){
                throw EBadArgumentsException("[RUNTIME_ERROR] Table '" + tableName + "' not found.");
            }

            shared_ptr<Table>& table1 = tableNickNames[firstAcronym];
            shared_ptr<Table>& table2 = tableNickNames[secondAcronym];

            if(!table1)
                table1 = database->tryGettingTableByNameCaseI(firstAcronym);
            if(!table1)
                throw EBadArgumentsException("[RUNTIME_ERROR] Table '" + firstAcronym + "' not found.");

            if(!table2)
                table2 = database->tryGettingTableByNameCaseI(secondAcronym);
            if(!table2)
                throw EBadArgumentsException("[RUNTIME_ERROR] Table '" + secondAcronym + "' not found.");

            int column1Index = table1->getColumnIndex(firstTableColumnNameInJoin);
            int column2Index = table2->getColumnIndex(secondTableColumnNameInJoin);

            if(column1Index == -1)
                throw EBadArgumentsException("[RUNTIME_ERROR] Column '" + firstTableColumnNameInJoin + "' not found in table '" + table1->getName() + "'");
            if(column2Index == -1)
                throw EBadArgumentsException("[RUNTIME_ERROR] Column '" + secondTableColumnNameInJoin + "' not found in table '" + table2->getName() + "'");


            string filterString1;
            for (int i = 0; i < table2->getTableRecords().size(); ++i) {
                string str = firstTableColumnNameInJoin + " = \"" +
                             table2->getTableRecords().at(i).getData().at(column2Index) + "\"";
                filterString1 = filterString1.empty() ? str : filterString1 + " OR " + str;
            }

            string filterString2;
            for (int i = 0; i < table1->getTableRecords().size(); ++i) {
                string str = secondTableColumnNameInJoin + " = \"" +
                             table1->getTableRecords().at(i).getData().at(column1Index) + "\"";
                filterString2 = filterString2.empty() ? str : filterString2 + " OR " + str;
            }

            Filter fil1 = Filter(table1, filterString1);
            table1 = fil1.getFilteredTable();

            Filter fil2 = Filter(table2, filterString2);
            table2 = fil2.getFilteredTable();


            if(mergedTables.empty())
                mergedTables.push_back(table1);
            mergedTables.push_back(table2);

        } else throw EBadArgumentsException("[SYNTAX_ERROR] Bad Syntax around: " + *it);
    }

    finalTable = mergedTables[0];
    for (int i = 1; i < mergedTables.size(); ++i) {
        finalTable = Table::getMergedTablesForJoin(finalTable,mergedTables.at(i));
    }

    cout << *finalTable;

    vector<string> selectedColumns = StringManipulator::splitString(argumentsStr, ',');

    cout << "WHERE STRING: " << whereStr << endl;
    // change every something.something into something in whereStr
    whereStr = regex_replace(whereStr, regex("(\\w+)\\.(\\w+)"), "$2");
    cout << "WHERE STRING: " << whereStr << endl;


    Filter f(finalTable, whereStr);
    shared_ptr<Table> filteredTable = f.getFilteredTable();


    cout << *getTableWithSelectedColumnsJoin(filteredTable, selectedColumns, tableNickNames);

    // obrisi error fajl
    ofstream errorFile("error.txt", ios::out);
    errorFile << "";
    errorFile.close();


    // upisi u ovaj fajl isto
    ofstream outFile("output.txt", ios::out);
    getTableWithSelectedColumnsJoin(filteredTable, selectedColumns, tableNickNames)->tablePrintRaw(outFile);
    outFile.close();


}

vector<int> Select::getVectorOfStartPositionOfEachTable(const map<string, shared_ptr<Table>> &tableNickNames) const {
    vector<int> startPositions;
    startPositions.reserve(tableNickNames.size());
    startPositions.push_back(0);
    int i = 1;
    for(auto it = tableNickNames.begin(); it != prev(tableNickNames.end()); ++it, ++i) {
        startPositions.push_back(it->second->getTableHeaders().size());
    }
    return startPositions;
}

void Select::checkForAmbiguity(const string &column, const shared_ptr<Table> &table) const {
    int cnt = 0;
    for (const string &tableColumn: table->getTableHeaders()) {
        vector<string> splitColumn = StringManipulator::instance().splitString(tableColumn, '.');
        string columnCheck = (splitColumn.size() == 1 ? splitColumn[0] : splitColumn[1]);

        if (regex_match(columnCheck, regex(column, regex_constants::icase))) {
            cnt++;
        }
    }
    if (cnt > 1)
        throw EBadArgumentsException("[RUNTIME_ERROR] Ambiguous column name \'" + column + "\'.");
}

shared_ptr<Table> Select::getTableWithSelectedColumnsJoin(shared_ptr<Table> tableArg, vector<string> selectedColumns,
                                                          map<string, shared_ptr<Table>> tableNicknames) const {
    if (selectedColumns.size() == 1 && selectedColumns.at(0) == "*") {
        return tableArg;
    }
    shared_ptr<Table> finalTable = make_shared<Table>(tableArg->getName());
    selectedColumns = getProcessedSelectedColumns(tableArg, selectedColumns);

    for (int i = 0; i < tableArg->getTableRecords().size(); i++)
        finalTable->addRecord(Record());

    for (const auto &selectedColumn: selectedColumns) {
        shared_ptr<Table> tempTable;
        if(regex_match(selectedColumn, regex("\\w+\\.\\w+", regex_constants::icase))) {
            vector<string> splitColumn = StringManipulator::instance().splitString(selectedColumn, '.');
            string tableAcronym = splitColumn[0];
            string column = splitColumn[1];
            if(tableNicknames.find(tableAcronym) == tableNicknames.end()) {
                throw EBadArgumentsException("[RUNTIME_ERROR] Table acronym '" + tableAcronym + "' not found.");
            }
            int indexOfAcronym = getIndexOfAcronymInMap(tableNicknames, tableAcronym);
            vector<int> startPositions = getVectorOfStartPositionOfEachTable(tableNicknames);
            int i = startPositions[indexOfAcronym];
            int k;
            if(indexOfAcronym + 1 < startPositions.size())
                k = startPositions[indexOfAcronym + 1];
            else
                k = tableArg->getTableHeaders().size();


            bool columnFound = false;
            for (; i < k; ++i) {
                if(regex_match(column, regex(tableArg->getTableHeaders()[i], regex_constants::icase))) {
                    tempTable = tableArg->getSubTableByIndex(i);
                    columnFound = true;
                    break;
                }
            }
            if(!columnFound)
                throw EBadArgumentsException("[RUNTIME_ERROR] Column '" + column + "' not found in table '" + tableArg->getName() + "'");


        } else {
            checkForAmbiguity(selectedColumn, tableArg);
            tempTable = tableArg->getSubTable(selectedColumn);
        }
        finalTable = Table::getMergedTable(finalTable, tempTable);
    }
    return finalTable;
}

int Select::getIndexOfAcronymInMap(const map<string, shared_ptr<Table>> tableNicknames, const string &acronym) {
    int i = 0;
    for (const auto &pair: tableNicknames) {
        if (regex_match(pair.first, regex(acronym, regex_constants::icase))) {
            return i;
        }
        i++;
    }
    throw EBadArgumentsException("[RUNTIME_ERROR] Table acronym '" + acronym + "' missing.");
}

shared_ptr<Table> Select::getTableWithSelectedColumns(shared_ptr<Table> tableArg, vector<string> selectedColumns) {
    if (selectedColumns.size() == 1 && selectedColumns.at(0) == "*") {
        return tableArg;
    }

    shared_ptr<Table> finalTable = make_shared<Table>(tableArg->getName());
    selectedColumns = getProcessedSelectedColumns(tableArg, selectedColumns);

    for (int i = 0; i < tableArg->getTableRecords().size(); i++)
        finalTable->addRecord(Record());

    for (const auto &selectedColumn: selectedColumns) {
        shared_ptr<Table> tempTable = tableArg->getSubTable(selectedColumn);
        finalTable = Table::getMergedTable(finalTable, tempTable);
    }
    return finalTable;
}

std::string Select::toLower(const string &str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

vector<string> Select::getProcessedSelectedColumns(shared_ptr<Table> tableArg, vector<string> selectedColumns) {
    vector<string> processedSelectedColumns;
    processedSelectedColumns.reserve(selectedColumns.size());
    for (const string &column: selectedColumns) {
        if (column == "*") {
            for (const string &header: tableArg->getTableHeaders())
                processedSelectedColumns.push_back(header);
        } else
            processedSelectedColumns.push_back(column);
    }

    for (const string &selectedColumn : processedSelectedColumns) {
        bool found = false;
        for (const string &header : tableArg->getTableHeaders()) {
            if (toLower(selectedColumn) == toLower(header)) {
                found = true;
                break;
            }
        }
        if (!found) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Column '" + selectedColumn + "' not found.");
        }
    }

    return processedSelectedColumns;
}

void Select::runtimeErrors(const string &input) const {
    throw EBadArgumentsException("[RUNTIME_ERROR] Bad syntax error.");
}

void Select::checkForSyntaxErrors(const string &query) const {
    if (regex_match(query, regex("^\\s*select(?:\\s+from.*|\\s*)", regex_constants::icase))) {
        throw EMissingArgumentsException("[SYNTAX_ERROR] SELECT has no arguments.");
    } else if (!regex_match(query, regex(".*\\s+from\\s*.*", regex_constants::icase))) {
        throw EMissingKeywordsException("[SYNTAX_ERROR] No FROM keyword specified.");
    } // table name not detected
    else if (!regex_search(query, regex(".*\\s+from\\s+\\w+\\s*", regex_constants::icase))) {
        throw EMissingArgumentsException("[SYNTAX_ERROR] FROM has no arguments.");
    }
    // multiple keywords
    if (regex_match(query, regex(".*\\bcreate\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with SELECT not allowed.");
    } else if (regex_match(query, regex(".*\\binsert\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with SELECT not allowed.");
    } else if (regex_match(query, regex(".*\\binto\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] INTO with SELECT not allowed.");
    } else if (regex_match(query, regex(".*\\bdelete\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with SELECT not allowed.");
    } else if (regex_match(query, regex(".*\\btable\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with SELECT not allowed.");
    } else if (regex_match(query, regex(".*\\bselect\\b.*\\bselect\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple SELECT keywords not allowed.");
    } else if (regex_match(query, regex(".*\\bfrom\\b.*\\bfrom\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple FROM keywords not allowed.");
    }



}

regex Select::getRegexPattern() const {
    return regex("^\\s*select\\s+((?:(?:\\w+\\.)?\\w+|\\*)(?:\\s*,\\s*(?:(?:\\w+\\.)?\\w+|\\*))*)\\s+from\\s+(\\w+(?:\\s+[^(?:where)])?(?:\\s+(?:inner\\s+)?join\\s+\\w+\\s+(?:\\w+\\s+)?on\\s+\\w+\\.\\w+\\s*\\=\\s*\\w+\\.\\w+|\\w+(?:\\s+\\w+)?)*\\s*)\\s*(?:where\\s+(((?:\\w+\\.)?\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\\\"\\w+\\\"|\\w+)(?:\\s+(and|or)\\s*((?:\\w+\\.)?\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\\\"\\w+\\\"|\\w+))*))?",
            regex_constants::icase);;
}

regex Select::getRegexForFindingTable() const {
    return regex("from\\s+(\\w+)", regex_constants::icase);
}
