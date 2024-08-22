

#include "CustomFormat.h"

CustomFormat::CustomFormat(const shared_ptr<Database> database) : Format(database) {}

shared_ptr<Database> CustomFormat::createDatabaseFromFile(const string &dirPath, const string &dbName) {
    string filePath = dirPath + dbName + ".wyl";
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw EFileNotOpen("[FILE_ERROR] No file at location: " + filePath);
    }

    shared_ptr<Database> database = make_shared<Database>(dbName);

    vector<shared_ptr<Table>> tables;
    bool readingTableColumns = false;
    bool readingData = false;
    shared_ptr<Table> currTable;
    string line;
    std::smatch matches;
    while (std::getline(inputFile, line)) {
        if(regex_search(line, matches,regex("^START_TABLE \"([a-zA-Z_]+)\""))) {
            currTable = make_shared<Table>(matches[1]);
            readingTableColumns = true;
            readingData = false;
            continue;
        } else if(line == "END_TABLE") {
            tables.push_back(currTable);
            readingTableColumns = false;
            readingData = false;
            continue;
        } else if(line == "START_DATA") {
            readingTableColumns = false;
            readingData = true;
            continue;
        } else if(line == "END_DATA") {
            readingTableColumns = false;
            readingData = false;
            continue;
        }

        if(readingTableColumns) {
            StringManipulator::instance().removeSpaces(line);
            currTable->addHeader(line.substr(1,line.length()-2));
        } else if(readingData) {
            if(regex_search(line, matches,regex("\\s*\\[\\\"([a-zA-Z_]+)\\\"\\] ADD \\[(\\'\\w+\\'(?:,\\'\\w+\\')*)\\]"))) {
                shared_ptr<Table> currTable;

                for(shared_ptr<Table> t : tables) {
                    if(t->getName() == matches[1]) {
                        currTable = t;
                        break;
                    }
                }
                if(!currTable) {
                    cout << "[RUNTIME_ERROR] Table " << matches[1] << " not found." << endl;
                    inputFile.close();
                    return nullptr;
                }

                vector<string> values = StringManipulator::instance().splitString(matches[2],',');
                Record r = Record();
                for(const string& str : values) {
                    string val = str.substr(1, str.length()-2);
                    r.addData(val);
                }
                currTable->addRecord(r);
            }
        }
    }

    for(const shared_ptr<Table> t : tables) {
        database->addTable(*t);
    }

    inputFile.close();
    return database;
}

void CustomFormat::printTableNames(stringstream &ss) const {
    for (const auto &tablePair: database->getTablePairs()) {
        ss << "START_TABLE \"" + tablePair.first + "\"" << endl;
        const auto &headers = tablePair.second->getTableHeaders();

        for (size_t i = 0; i < headers.size(); ++i) {
            ss << ("   \"" + headers[i] + "\"") << endl;
        }
        ss << "END_TABLE" << endl;
    }
}

void CustomFormat::printDataInsertion(stringstream &ss) const {
    int maxLength = calculateMaxColumnNameLength(database->getAllHeaders());
    ss << "START_DATA" << endl;
    for (const auto &tablePair: database->getTablePairs()) {

        for (const Record &record: tablePair.second->getTableRecords()) {



            ss << std::setw(maxLength + 10) << std::right << ("[\"" + tablePair.first + "\"] ADD [");
            for (int i = 0; i < record.getData().size(); i++) {
                ss << "\'" << record.getData()[i];
                if (i < record.getData().size() - 1)
                    ss << "\',";
                else
                    ss << "\']" << std::endl;
            }
        }
    }
    ss << "END_DATA";
}

string CustomFormat::getFileExtension() const {
    return ".wyl";
}

string CustomFormat::getDir() const {
    return "CustomFormatExports/";
}
