#ifndef PRVIPROJEKAT_QUERYEDITOR_H
#define PRVIPROJEKAT_QUERYEDITOR_H


#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "Database.h"
#include "Select.h"
#include "Create.h"
#include "Drop.h"
#include "Insert.h"
#include "Update.h"
#include "Delete.h"

using namespace std;


class QueryEditor {
public:
    QueryEditor(shared_ptr<Database> d);

    QueryEditor(const QueryEditor &) = delete;
    QueryEditor(QueryEditor &&) = delete;

    QueryEditor &operator=(const QueryEditor &) = delete;
    QueryEditor &operator=(QueryEditor &&) = delete;


    void start();


// throws Errors, should be caught when being called
void startQuery(const string &query);

private:
    shared_ptr<Database> database;

    const string KEYWORDCOLOR =
            StringManipulator::instance().RGB(184, 51, 106) + StringManipulator::instance().BOLDCOLOR();
    const string  COLOMNCOLOR =
            StringManipulator::instance().RGB(70, 177, 201) + StringManipulator::instance().BOLDCOLOR();
    const string   TABLECOLOR =
            StringManipulator::instance().RGB(66, 255, 186) + StringManipulator::instance().BOLDCOLOR();
//    const string   SPECCOLOR =
//            StringManipulator::instance().RGB(48, 16, 20) + StringManipulator::instance().BOLDCOLOR();
    const string   QUOTECOLOR =
            StringManipulator::instance().RGB(238, 108, 77) + StringManipulator::instance().BOLDCOLOR();
    const string   DQUOTECOLOR =
            StringManipulator::instance().RGB(182, 238, 166) + StringManipulator::instance().BOLDCOLOR();
    const string   RESETCOLOR = StringManipulator::instance().RESETCOLOR();



    string colorSingleQuotations(const std::string& str) const;

    string colorDoubleQuotations(const std::string& str) const;

    string colorQuotations(const std::string& str) const;


    void printHeaderAndClearConsole() const;

    string colorVectorValuesInString(string str, vector<string> vect, const string& COLOR) const;

    void paintingConsole(string& lineWithLineFeed) const;

    vector<string> editor() const;


    //<editor-fold desc="Clear Console">
    void clearConsole() const;
    //</editor-fold>
};


#endif //PRVIPROJEKAT_QUERYEDITOR_H
