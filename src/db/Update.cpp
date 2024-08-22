#include "Update.h"

Update::Update(const string &input, shared_ptr<Database> database) : Statement(input, database) {}


void Update::executingQuery(const smatch &matches) const {
    string tableName = matches[1];
    string setArgStr = matches[2];
    string whereStr = matches[3];

    vector<string> setArgs = StringManipulator::instance().splitString(setArgStr, ',');
    for (string &str: setArgs) {
        str = StringManipulator::instance().trim(str);
    }

    Filter f(this->table, whereStr);
    shared_ptr<Table> filterTable = f.getFilteredTable();

    vector<int> indices;
    indices.reserve(filterTable->getTableRecords().size());
    for (const Record &rec: filterTable->getTableRecords()) {
        indices.push_back(table->getRecordIndex(rec.getData()));
    }

    for (int index: indices) {
        for (const string &setArg: setArgs) {
            std::smatch matches1;
            if (regex_search(setArg, matches1,
                             regex("^\\s*(\\w+)\\s*\\=\\s*[\'\"](\\w*|\\s)[\'\"]", regex_constants::icase))) {
                string columnName = matches1[1];
                string columnValue = matches1[2];

                StringManipulator::instance().trim(columnName);
                StringManipulator::instance().trim(columnValue);

                bool found = false;
                for (const auto &fil: filterTable->getTableHeaders()) {
                    if (regex_match(columnName, regex(fil, regex_constants::icase)))
                        found = true;
                }
                if (!found)
                    throw EBadArgumentsException("[RUNTIME_ERROR] Column \'" + columnName + "\' not found in table.");


                int columnIndex = -1;
                for (int i = 0; i < filterTable->getTableHeaders().size(); i++) {
                    if (regex_match(columnName, regex(filterTable->getTableHeaders().at(i), regex_constants::icase))) {
                        columnIndex = i;
                        break;
                    }
                }
                table->getRecordByIndex(index).getDataReference()[columnIndex] = columnValue;

            }
        }
    }

    // obrisi error fajl
    ofstream errorFile("error.txt", ios::out);
    errorFile << "";
    errorFile.close();


    // upisi u ovaj fajl isto
    ofstream outFile("output.txt", ios::out);
    table->tablePrintRaw(outFile);
    outFile.close();

    cout << *table;

    printAffectedRowsMessage(indices.size());
}


void Update::printAffectedRowsMessage(int num) const {
    string msg;
    if (num == 0) {
        msg = "No rows affected through this update.";
    } else if (num == 1) {
        msg = "1 row was affected through this update.";
    } else {
        msg = to_string(num) + " rows were affected through this update.";
    }
    StringManipulator::instance().newMessage(msg);
}

void Update::runtimeErrors(const string &input) const {
    throw EBadArgumentsException("[RUNTIME_ERROR] Bad update syntax.");
}

void Update::checkForSyntaxErrors(const string &query) const {
    // set not detected
    if (!regex_match(query, regex(".*\\s+set\\s*.*", regex_constants::icase))) {
        throw EMissingKeywordsException("[SYNTAX_ERROR] No SET keyword specified.");
    } // table name not detected
    else if (!regex_search(query, regex(".*\\s+set\\s+\\w+\\s*", regex_constants::icase))) {
        throw EMissingArgumentsException("[SYNTAX_ERROR] SET has no arguments.");
    }
    // multiple keywords
    if (regex_match(query, regex(".*\\bcreate\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with UPDATE not allowed.");
    } else if (regex_match(query, regex(".*\\binsert\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with UPDATE not allowed.");
    } else if (regex_match(query, regex(".*\\binto\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] INTO with UPDATE not allowed.");
    } else if (regex_match(query, regex(".*\\bdelete\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with UPDATE not allowed.");
    } else if (regex_match(query, regex(".*\\btable\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with UPDATE not allowed.");
    } else if (regex_match(query, regex(".*\\bupdate\\b.*\\bupdate\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple UPDATE keywords not allowed.");
    }
    // update has no arguments
    if (!regex_search(query, regex("^\\s*update\\s+(\\w+)", regex_constants::icase))) {
        throw EMissingArgumentsException("[SYNTAX_ERROR] UPDATE has no arguments.");
    }
}

regex Update::getRegexForFindingTable() const {
    return regex("^\\s*update\\s+(\\w+)", regex_constants::icase);
}

regex Update::getRegexPattern() const {
    return regex(
            "^\\s*update\\s+([a-zA-Z_]*)\\s+set\\s+(\\w+\\s*\\=\\s*(?:\\\"(?:\\w*|\\s)\\\"|\\'(?:\\w*|\\s)\\')\\s*(?:\\,\\s*\\w+\\s*\\=\\s*(?:\\\"(?:\\w*|\\s)\\\"|\\'(?:\\w*|\\s)\\')\\s*)*)(?:where\\s+((\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+)(?:\\s+(and|or)\\s*(\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+))*))?",
            regex_constants::icase);
}


