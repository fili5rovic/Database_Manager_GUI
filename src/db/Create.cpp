#include "Create.h"

Create::Create(const string &input, shared_ptr<Database> database) : Statement(input, database) {}

void Create::execute() {
    table = make_shared<Table>(getTableNameFromCreateQuery(inputQuery));
    handleQuery(inputQuery);
}

void Create::executingQuery(const smatch &matches) const {
    vector<string> tableColumns = StringManipulator::instance().splitString(matches[2], ',');
    for (string &columnName: tableColumns) {
        StringManipulator::removeSpaces(columnName);
        table->addHeader(columnName);
    }
    if (this->database) {
        if (this->database->tryGettingTableByNameCaseI(table->getName()))
            throw EBadArgumentsException("[RUNTIME_ERROR] Table with identical name exists inside a database!");
        this->database->addTable(*this->table);
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
}

string Create::getTableNameFromCreateQuery(const string &query) const {
    checkForSyntaxErrors(query);

    std::smatch matches;
    vector<string> names;                           // it was s+ in the end instead of s*
    if (regex_search(query, matches, regex("^\\s*create\\s+table\\s+(\\w+)(?:s*|\\()", regex_constants::icase))) {
        if (!regex_match(matches[1].str(), regex("[a-zA-Z_]+", regex_constants::icase)))
            throw EBadArgumentsException("[RUNTIME_ERROR] Table name " + matches[1].str() + " not allowed.");
        return matches[1];
    }
    throw EBadArgumentsException("[RUNTIME_ERROR] Table name not allowed.");
}

void Create::runtimeErrors(const string &input) const {
    std::smatch matches;
    if (regex_match(input, regex("\\s*create\\s+table\\s+\\w+\\s*", regex_constants::icase))) {
        throw EBadArgumentsException("[RUNTIME_ERROR] No table column names passed.");
    }
    if (!regex_match(this->table->getName(), regex("[a-zA-Z_]+", regex_constants::icase))) {
        throw EBadArgumentsException("[RUNTIME_ERROR] Invalid table name.");
    }

    throw EBadArgumentsException("[SYNTAX_ERROR] Table creation error.");
}

void Create::checkForSyntaxErrors(const string &query) const {
    if (!regex_match(query, regex(".*\\s+table.*", regex_constants::icase)))
        throw EMissingKeywordsException("[SYNTAX_ERROR] No TABLE keyword");

    // multiple keywords
    if (regex_match(query, regex(".*\\binsert\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with CREATE not allowed within the same statement.");
    } else if (regex_match(query, regex(".*\\bselect\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] SELECT with CREATE not allowed within the same statement.");
    } else if (regex_match(query, regex(".*\\bdelete\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with CREATE not allowed within the same statement.");
    } else if (regex_match(query, regex(".*\\bcreate\\b.*\\bcreate\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple CREATE keywords not allowed within the same statement.");
    } else if (regex_match(query, regex(".*\\btable\\b.*\\btable\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple TABLE keywords not allowed within the same statement.");
    }
}

regex Create::getRegexPattern() const {
    return regex("^\\s*create\\s+table\\s+([a-zA-Z_]+)\\s*\\(\\s*(\\w+\\s*(?:,\\s*\\w+\\s*)*)\\s*\\)",
                 regex_constants::icase);
}

regex Create::getRegexForFindingTable() const {
    return regex("^\\s*create\\s+table\\s+(\\w+)(?:s*|\\()", regex_constants::icase);
}
