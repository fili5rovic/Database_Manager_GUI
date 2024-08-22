//
// Created by fili5 on 19.4.2024..
//

#include "SQLFormat.h"

SQLFormat::SQLFormat(shared_ptr<Database> database) : Format(database) {}

void SQLFormat::printTableNames(stringstream &ss) const {
    for (const auto &tablePair: database->getTablePairs()) {
        ss << "CREATE TABLE IF NOT EXISTS \"" + tablePair.first + "\" (" << std::endl;
        const auto &headers = tablePair.second->getTableHeaders();

        // Calculate the maximum length of the column names
        int max_length = 0;
        for (const auto &header : headers) {
            if (header.length() > max_length) {
                max_length = header.length();
            }
        }

        for (size_t i = 0; i < headers.size(); ++i) {
            ss << std::setw(max_length + 5) << std::right << ("\"" + headers[i] + "\"") << "\tVARCHAR(50) NOT NULL";
            if (i < headers.size() - 1)
                ss << ',';
            ss << std::endl;
        }
        ss << ");" << std::endl;
    }
}

void SQLFormat::printDataInsertion(stringstream &ss) const {
    for (const auto &tablePair: database->getTablePairs()) {
        for (const Record &record: tablePair.second->getTableRecords()) {
            ss << "INSERT INTO \"" << tablePair.first << "\" VALUES (";
            for (int i = 0; i < record.getData().size(); i++) {
                ss << "\'" << record.getData()[i];
                if (i < record.getData().size() - 1)
                    ss << "\',";
                else
                    ss << "\');" << std::endl;
            }
        }
    }
}

string SQLFormat::getFileExtension() const {
    return ".sql";
}

string SQLFormat::getDir() const {
    return "SQLFormatExports/";
}
