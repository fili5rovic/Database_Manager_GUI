#ifndef PRVIPROJEKAT_FILTER_H
#define PRVIPROJEKAT_FILTER_H

#include <regex>
#include "Condition.h"
#include "Table.h"
#include "Equal.h"
#include "NotEqual.h"

class Filter {
public:

    Filter(const shared_ptr<Table> t, const string &whereArgsString);

    shared_ptr<Table> getFilteredTable();


private:
    // outer vector stores conditions seperated by OR, inner by AND
    vector<vector<shared_ptr<Condition>>> conditions;
    shared_ptr<Table> table;


    // creates conditions, only called during object creation
    void init(const shared_ptr<Table> t, const string &whereArgs);

    void updateTableRows() const;


    const string getCorrectMatch(const std::smatch& matches) const {
        for(int i = 2; i < matches.length();i++) {
            if(matches[i].length() > 0)
                return matches[i];
        }
        return "-1";
    }


    void setHeaderAndNameFromParameterTable(const shared_ptr<Table> t) {
        table = make_shared<Table>("RESULT");
        for(const string& header : t->getTableHeaders()) {
            table->addHeader(header);
        }
    }



    // gets normal strings
    static vector<string> getAndStringsFromOrStrings(const string &whereStr) {
        std::regex regex_pattern(R"(\s+and\s+)", regex_constants::icase);

        std::sregex_token_iterator iter(whereStr.begin(), whereStr.end(), regex_pattern, -1);
        std::sregex_token_iterator end;

        vector<string> orStrings;

        while (iter != end) {
            orStrings.push_back(*iter);
            ++iter;
        }
        return orStrings;
    }

    // returns strings seperated by AND
    static vector<string> getOrStringsFromWhere(const string &whereStr) {
        std::regex regex_pattern(R"(\s+or\s+)", regex_constants::icase);

        std::sregex_token_iterator iter(whereStr.begin(), whereStr.end(), regex_pattern, -1);
        std::sregex_token_iterator end;

        vector<string> orStrings;

        while (iter != end) {
            orStrings.push_back(*iter);
            ++iter;
        }
        return orStrings;
    }


};


#endif //PRVIPROJEKAT_FILTER_H
