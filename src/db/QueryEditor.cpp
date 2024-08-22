#include "QueryEditor.h"

QueryEditor::QueryEditor(shared_ptr<Database> d) : database(d) {}

void QueryEditor::start() {
    vector<string> inputQueries = editor();
    for (auto inputQuery: inputQueries) {
        StringManipulator::removeDuplicateSpaces(inputQuery);
        if (inputQuery.empty())
            return;
        cout << endl;
//        cout << endl << "INPUT QUERY: " << inputQuery << endl;
        try {
            startQuery(inputQuery);
        } catch (exception& e) {
            cout << e.what() << endl;
            cout << "Press any key to continue..." << endl;
            getch();
        }
    }
}

string QueryEditor::colorSingleQuotations(const string &str) const {
    string result;
    bool openQuot = false;
    for(const char& c : str) {
        if(c == '\'') {
            openQuot = !openQuot;
            if(openQuot)
                result += QUOTECOLOR;
            else {
                result += c + RESETCOLOR;
                continue;
            }
        }
        result += c;
    }
    return result;
}

string QueryEditor::colorDoubleQuotations(const string &str) const {
    string result;
    bool openQuot = false;
    for(const char& c : str) {
        if(c == '\"') {
            openQuot = !openQuot;
            if(openQuot)
                result += DQUOTECOLOR;
            else {
                result += c + RESETCOLOR;
                continue;
            }
        }
        result += c;
    }
    return result;
}

string QueryEditor::colorQuotations(const string &str) const {
    string res = str;
    res = colorDoubleQuotations(res);
    res = colorSingleQuotations(res);
    return res;
}

void QueryEditor::printHeaderAndClearConsole() const {
    clearConsole();
    cout << "\033[47m" << "  SQL Query Editor                                       0" << "\033[41m" << " X "
         << StringManipulator::instance().RESETCOLOR() << endl;
}

string QueryEditor::colorVectorValuesInString(string str, vector<string> vect, const string &COLOR) const {
    regex pattern;

    // Sort keywords in descending order of length
    std::sort(vect.begin(), vect.end(), [](const string &a, const string &b) {
        return a.length() > b.length();
    });

    vector<string> keywordReplacementStrings;
    keywordReplacementStrings.reserve(vect.size());
    for (const string &reg: vect) {
        keywordReplacementStrings.push_back(COLOR + reg + RESETCOLOR);
    }

    for (int i = 0; i < vect.size(); i++) {
        pattern = regex("\\b" + vect[i] + "\\b", regex_constants::icase);
        str = regex_replace(str, pattern, keywordReplacementStrings[i]);
    }
    return str;
}

void QueryEditor::paintingConsole(string &lineWithLineFeed) const {
    vector<string> keywords{"SELECT", "FROM", "WHERE", "INSERT", "INTO", "CREATE", "SHOW","TABLES","TABLE", "DROP", "UPDATE" ,"SET", "DELETE", "JOIN","INNER", "AND", "OR", "VALUES","ON"};
    lineWithLineFeed = colorVectorValuesInString(lineWithLineFeed, keywords, KEYWORDCOLOR);
    if (regex_search(lineWithLineFeed, regex("from|update|into", regex_constants::icase)))
        lineWithLineFeed = colorVectorValuesInString(lineWithLineFeed, database->getAllTableNames(), TABLECOLOR);
    if (regex_search(lineWithLineFeed, regex("select|where|set", regex_constants::icase)))
        lineWithLineFeed = colorVectorValuesInString(lineWithLineFeed, database->getAllHeaders(), COLOMNCOLOR);
    lineWithLineFeed = colorQuotations(lineWithLineFeed);
}

vector<string> QueryEditor::editor() const {
    printHeaderAndClearConsole();
    string sqlQuery;
    string line;
    string lineWithLineFeed;
    int lineCounter = 0;
    while (true) {
        cout << StringManipulator::instance().GRAYCOLOR() << to_string(++lineCounter)
             << StringManipulator::instance().RESETCOLOR() << "  ";
        getline(std::cin, line);

        string lineWithNum = StringManipulator::instance().GRAYCOLOR() + to_string(lineCounter) +
                             StringManipulator::instance().RESETCOLOR() + "  " + line;

        lineWithLineFeed += (lineWithLineFeed.empty() ? lineWithNum : '\n' + lineWithNum);
        printHeaderAndClearConsole();

        paintingConsole(lineWithLineFeed);

        cout << lineWithLineFeed << endl;
        if (line.empty())
            break;
        sqlQuery.append(sqlQuery.empty() ? line : (" " + line));
    }

    vector<string> sqlQueries = StringManipulator::splitString(sqlQuery, ';');
    return sqlQueries;
}

void QueryEditor::startQuery(const string &query) {
    std::shared_ptr<Statement> type = nullptr;
    if (regex_match(query, regex("^\\s*select.*", regex_constants::icase))) {
        type = std::make_shared<Select>(query, database);
    } else if (regex_match(query, regex("^\\s*create.*", regex_constants::icase))) {
        type = std::make_shared<Create>(query, database);
    } else if (regex_match(query, regex("^\\s*drop.*", regex_constants::icase))) {
        type = std::make_shared<Drop>(query, database);
    } else if(regex_match(query, regex("^\\s*insert.*",regex_constants::icase))) {
        type = std::make_shared<Insert>(query, database);
    } else if(regex_match(query, regex("^\\s*update.*",regex_constants::icase))) {
        type = std::make_shared<Update>(query, database);
    } else if(regex_match(query, regex("^\\s*delete.*",regex_constants::icase))) {
        type = std::make_shared<Delete>(query, database);
    } else if(regex_match(query, regex("^\\s*show\\s+tables\\s*", regex_constants::icase))) {
        // obrisi error fajl
        ofstream errorFile("error.txt", ios::out);
        errorFile << "";
        errorFile.close();

        // upisi u ovaj fajl isto
        ofstream outFile("output.txt", ios::out);
        database->printDatabaseRaw(outFile);
        outFile.close();

        cout << *database;
        return;
    }
    if (!type)
        throw ENoKeywordsException("[SYNTAX_ERROR] No keywords detected. Can't detect query.");

    type->execute();
}

void QueryEditor::clearConsole() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
