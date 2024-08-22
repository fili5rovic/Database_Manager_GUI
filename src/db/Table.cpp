#include "Table.h"


Table::Table(const string &name) : name(name), records(), header() {}

Table::Table(const Table &t) {
    copy(t);
}

Table::Table(Table &&t) {
    move(t);
}

Table &Table::operator=(const Table &t) {
    copy(t);
    return *this;
}

Table &Table::operator=(Table &&t) {
    move(t);
    return *this;
}

ostream &operator<<(ostream &os, const Table &t) {
    t.tablePrint(os);
    return os;
}

void Table::addRecord(const Record &r) {
    records.push_back(r);
}

void Table::addHeader(const string &s) {
    header.push_back(s);
}

Record &Table::getRecordByIndex(int index) {
    if(index < 0 || index >= records.size()) {
        throw std::out_of_range("Index out of range");
    }
    return records[index];
}

int Table::getRecordIndex(const vector<string> &values) const {
    for(int i = 0; i < records.size(); i++) {
        bool same = true;
        for(int j = 0; j < records[i].getData().size() ; j++) {
            if(values[j] != records[i].getData()[j]) {
                same = false;
                break;
            }
        }
        if(same)
            return i;
    }
    return -1;
}

void Table::removeRecordFromIndex(int index) {
    if(index < 0 || index >= records.size())
        throw std::out_of_range("[RUNTIME_ERROR] Index out of range");
    records.erase(records.begin() + index);
}

int Table::getColumnIndex(const string &columnName) const {
    for(int i = 0; i < header.size(); i++) {
        if(regex_match(header[i], regex(columnName, regex_constants::icase)))
            return i;
    }
    return -1;
}

shared_ptr<Table> Table::getSubTable(const string &colName) const {
    int columnIndex = -1;
    for (int i = 0; i < this->header.size(); i++) {
        if (regex_match(this->header.at(i), regex(colName, regex_constants::icase))) {
            columnIndex = i;
            break;
        }
    }

    return getSubTableByIndex(columnIndex);
}

shared_ptr<Table> Table::getSubTableByIndex(int columnIndex) const {
    if (columnIndex == -1) {
        throw EBadArgumentsException("[RUNTIME_ERROR] Column " + this->header.at(columnIndex) + " does not exist inside " + this->name);
    }
    shared_ptr<Table> t = make_shared<Table>(this->getName());
    t->addHeader(this->header.at(columnIndex));
    for (const Record& record: this->records) {
        Record r = Record();
        r.addData(record.getData().at(columnIndex));
        t->addRecord(r);
    }
    return t;
}

shared_ptr<Table> Table::getMergedTable(const shared_ptr<Table> t1, const shared_ptr<Table> t2) {
    shared_ptr<Table> finalTable = make_shared<Table>(t1->name);
    for (const string &h1: t1->header) {
        finalTable->addHeader(h1);
    }

    for (const string &h2: t2->header) {
        finalTable->addHeader(h2);
    }

    for (int i = 0; i < t1->records.size(); i++) {
        Record record = Record();
        for (const string& str1: t1->records.at(i).getData()) {
            record.addData(str1);
        }
        for (const string& str2: t2->records.at(i).getData()) {
            record.addData(str2);
        }
        finalTable->addRecord(record);
    }

    return finalTable;
}

shared_ptr<Table> Table::getMergedTablesForJoin(const shared_ptr<Table> t1, const shared_ptr<Table> t2) {
    shared_ptr<Table> finalTable = make_shared<Table>(t1->name);
    for (const string &h1: t1->header) {
        finalTable->addHeader(h1);
    }

    for (const string &h2: t2->header) {
        finalTable->addHeader(h2);
    }

    for (int i = 0; i < min(t1->records.size(),t2->records.size()); i++) {
        Record record = Record();
        for (const string& str1: t1->records.at(i).getData()) {
            record.addData(str1);
        }
        for (const string& str2: t2->records.at(i).getData()) {
            record.addData(str2);
        }
        finalTable->addRecord(record);
    }

//        for (int i = 0; i < max(t1->records.size(), t2->records.size()); i++) {
//            finalTable->addRecord(Record());
//        }
//
//        for (int i = 0; i < t1->records.size(); i++) {
//            Record record = Record();
//            for (const string& str1: t1->records.at(i).getData()) {
//                record.addData(str1);
//            }
//            finalTable->getRecordByIndex(i).getDataReference() = record.getData();
//        }
//
//        for (int i = 0; i < t2->records.size(); i++) {
//            Record record = Record();
//            for (const string& str2: t2->records.at(i).getData()) {
//                record.addData(str2);
//            }
//            vector<string> data = record.getData();
//            for(const string& s : data) {
//                finalTable->getRecordByIndex(i).getDataReference().push_back(s);
//            }
//        }

    return finalTable;
}

const string &Table::getName() const {
    return this->name;
}

const vector<string> &Table::getTableHeaders() const {
    return this->header;
}

const vector<Record> &Table::getTableRecords() const {
    return this->records;
}

vector<Record> &Table::getTableRecordsByReference() {
    return records;
}

void Table::copy(const Table &t) {
    this->name = t.name;
    this->records = vector<Record>();
    for (const Record &s: t.records) {
        this->records.push_back(s);
    }

    this->header = vector<string>();
    for (const string &s: t.header) {
        this->header.push_back(s);
    }
}

void Table::move(Table &t) {
    this->name = t.name;
    this->records = t.records;
    this->header = t.header;
    t.records = vector<Record>();
    t.name = "";
}

void Table::tablePrint(ostream &os) const {
    const char verticalLine = '\xB3';

    vector<int> width = getWidthVectorForTablePrinting();
    if(width.size() == 1){
        int tableNameLen = this->name.length();
        width[0] = max(width[0],tableNameLen+2);
    }

    upperPartTablePrint(width, os);

    int headerSize = std::accumulate(width.begin(), width.end(), 0);
    headerSize += width.size() - 1;

    int paddingHeader = headerSize - name.size();
    int leftPaddingHeader = paddingHeader / 2;
    int rightPaddingHeder = paddingHeader - leftPaddingHeader + 1;
    os << verticalLine << "\033[35m"
         << std::setw(leftPaddingHeader + name.size()) << std::right << name << "\033[0m"
         << std::setw(rightPaddingHeder) << verticalLine << endl;

    middlePartTablePrint(width, os, true);

    os << verticalLine;
    for (int i = 0; i < header.size(); i++) {
        int padding = width[i] - header[i].size();
        int leftPadding = padding / 2;
        int rightPadding = padding - leftPadding + 1;
        os << std::setw(leftPadding + header[i].size()) << std::right << header[i] << std::setw(rightPadding)
             << verticalLine;
    }
    os << endl;

    if(records.empty())
        bottomPartTablePrint(width, os);
    else
        middlePartTablePrint(width, os);

    for (auto it = records.begin(); it != records.end(); ++it) {
        const Record& record = *it;
        os << verticalLine;
        for (int i = 0; i < record.getData().size(); i++) {
            int padding = width[i] - record.getData()[i].size();
            int leftPadding = padding / 2;
            int rightPadding = padding - leftPadding + 1;
            os << std::setw(leftPadding + record.getData()[i].size()) << std::right << record.getData()[i]
                 << std::setw(rightPadding)
                 << verticalLine;
        }
        os << endl;
        if (it < records.end() - 1)
            middlePartTablePrint(width, os);
        else
            bottomPartTablePrint(width, os);
    }

}

void Table::upperPartTablePrint(vector<int> width, ostream &os) const {
    const char topLeftCorner = '\xDA';
    const char topRightCorner = '\xBF';
    const char horizontalLine = '\xC4';

    os << topLeftCorner;
    for (int i = 0; i < width.size() - 1; ++i) {
        for (int j = 0; j < width[i]; ++j) {
            os << horizontalLine;
        }
        os << horizontalLine;
    }
    for (int j = 0; j < width[width.size() - 1]; ++j) {
        os << horizontalLine;
    }
    os << topRightCorner << endl;
}

void Table::middlePartTablePrint(vector<int> width,ostream &os, bool up) const {
    const char middleLeftCorner = '\xC3';
    const char middleDown = '\xC2';
    const char middleCenter = '\xC5';
    const char middleRightCorner = '\xB4';
    const char horizontalLine = '\xC4';

    os << middleLeftCorner;
    for (int i = 0; i < width.size() - 1; ++i) {
        for (int j = 0; j < width[i]; ++j) {
            os << horizontalLine;
        }
        os << (up ? middleDown : middleCenter);
    }
    for (int j = 0; j < width[width.size() - 1]; ++j) {
        os << horizontalLine;
    }
    os << middleRightCorner << endl;
}

void Table::bottomPartTablePrint(vector<int> width, ostream &os) const {
    const char bottomLeftCorner = '\xC0';
    const char bottomRightCorner = '\xD9';
    const char horizontalLine = '\xC4';
    const char middleUp = '\xC1';

    os << bottomLeftCorner;
    for (int i = 0; i < width.size() - 1; ++i) {
        for (int j = 0; j < width[i]; ++j) {
            os << horizontalLine;
        }
        os << middleUp;
    }
    for (int j = 0; j < width[width.size() - 1]; ++j) {
        os << horizontalLine;
    }
    os << bottomRightCorner << endl;
}

vector<int> Table::getWidthVectorForTablePrinting(int padding) const {
    vector<int> width;
    width.resize(header.size());
    fill(width.begin(), width.end(), 0);

    for (int i = 0; i < width.size(); ++i) {
        width[i] = header[i].size();
    }

    for (const Record &record: records) {
        for (int j = 0; j < record.getData().size(); j++) {
            int strSize = record.getData()[j].length();
            width[j] = max(width[j], strSize);
        }
    }

    for_each(width.begin(), width.end(), [padding](int &value) {
        value += padding;
    });

    return width;
}

void Table::tablePrintRaw(ostream &os) const {
    os << name << endl;
    for (const auto & h : header) {
        os  << h  << '|';
    }
    os << endl;

    for (auto it = records.begin(); it != records.end(); ++it) {
        const Record& record = *it;
        for (int i = 0; i < record.getData().size(); i++) {
            os << record.getData()[i] << '|';
        }
        os << endl;
    }
}
