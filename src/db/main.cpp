#include <iostream>
#include "Database.h"
#include "QueryEditor.h"
#include "Equal.h"
#include "SQLFormat.h"
#include "CustomFormat.h"
#include <vector>
#include <filesystem>

using namespace std;

const string GRAY = "\033[37m";
const string RESET = "\033[0m";


shared_ptr<Database> database = nullptr;

void printActualInput(string selectedMenuOptionString, int substrNum = 4, int toTheRight = 3) {
    substrNum = (selectedMenuOptionString.length() > substrNum ? substrNum : 0);
    const string moveCursorUp = "\033[F";
    const string moveCursorRight = "\033[C";
    const string userInputColor = StringManipulator::instance().CYANCOLOR();

    cout << moveCursorUp;
    for (int i = 0; i < toTheRight; ++i) {
        cout << moveCursorRight;
    }

    cout << userInputColor;
    cout << selectedMenuOptionString.substr(substrNum);
    cout << RESET << endl;
}

shared_ptr<Database> createNewDatabaseMenu() {
    string databaseName, menuHeader, msg;
    vector<string> opts;

    StringManipulator::instance().newMessage("Insert database name:");
    cout << GRAY << "-> " << RESET;
    cin >> databaseName;
    printActualInput(databaseName, 0);

    shared_ptr<Database> database = make_shared<Database>(databaseName);
    int opt = -1;
    while (opt != 0) {
        menuHeader = "     Add new table to \033[35m'" + databaseName + "'\033[0m?    ";
        opts = {menuHeader, "[1] Yes", "[0] No"};
        int formula = menuHeader.length() - 8 + menuHeader.length() % 2 - 1; // za pravilnu velicinu tabele
        StringManipulator::instance().newMenu(formula, opts);
        cout << GRAY << "-> " << RESET;
        cin >> opt;


        if (opt == 1) {
            printActualInput(opts[1]);
            StringManipulator::instance().newMessage("Insert table name:");
            cout << GRAY << "-> " << RESET;
            string tableName;
            cin >> tableName;

            printActualInput(tableName, 0);

            Table t = Table(tableName);
            string header;
            vector<string> unos;
            while (true) {
                msg = "     Insert columns for \033[35m" + tableName + "\033[0m    ";
                StringManipulator::instance().newMessage(msg, msg.length() - 8 + msg.length() % 2 - 1);
                cin.ignore();
                cout << GRAY << "-> " << RESET;
                getline(std::cin, header);
                printActualInput(header, 0);
                if (header.empty())
                    continue;
                unos = StringManipulator::splitString(header);
                for (const string &u: unos) {
                    t.addHeader(u);
                }
                break;
            }


            msg = "     Insert data for \033[35m" + tableName + "\033[0m    ";
            StringManipulator::instance().newMessage(msg, msg.length() - 8 + msg.length() % 2 - 1);
            for (const string &u: unos) {
                cout << StringManipulator::instance().BOLDCOLOR() << u << ' ';
            }
            cout << endl;

            string row;
            while (true) {
                getline(std::cin, row);
                if (row.empty())
                    break;
                Record r = Record();
                vector<string> rowData = StringManipulator::splitString(row);

                if (rowData.size() != unos.size()) {
                    cout << "\033[F";
                    cout << StringManipulator::instance().REDCOLOR();
                    cout << row << endl;
                    cout << "[ERROR] Given row has " << (rowData.size() > unos.size() ? "more" : "less")
                         << " values than columns." << endl;
                    cout << StringManipulator::instance().RESETCOLOR();
                    cout << "Press any key to continue..." << endl;
                    getch();
                    cout << "\033[F\033[F\033[F";
                    cout << StringManipulator::instance().printspace(30) << endl;
                    cout << StringManipulator::instance().printspace(50) << endl;
                    cout << StringManipulator::instance().printspace(30) << endl;
                    cout << "\033[F\033[F\033[F";
                    continue;
                }
                printActualInput(row, 0, 0);

                for (const string &d: rowData) {
                    r.addData(d);
                }
                t.addRecord(r);
            }

            msg = "Table " + tableName + " successfully created.";
            StringManipulator::instance().newMessageGreen(msg);

            database->addTable(t);
        } else {
            printActualInput(opts[2]);
        }

    }

    string successfulCreation = "Database \'" + databaseName + "\' successfully created.";
    StringManipulator::instance().newMessageGreen(successfulCreation);
    return database;
}

int getFileCount(const std::string &path) {
    int fileCount = 0;
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_regular_file(entry.status())) {
            ++fileCount;
        }
    }
    return fileCount;
}

shared_ptr<Database> loadDatabase() {
    std::string path = "./CustomFormatExports/";
    vector<string> options;
    options.reserve(getFileCount(path) + 1);

    options.push_back("Choose database");
    int cnt = 1;
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        string dbName = entry.path().string().substr(path.length());
        dbName = dbName.substr(0, dbName.length() - 4);
        options.push_back("[" + to_string(cnt++) + "] " + dbName);
    }
    options.push_back("[0] Back.");

    int maxLineLength = 15;
    for (const auto &opt: options) {
        if (opt.length() > maxLineLength)
            maxLineLength = opt.length();
    }

    maxLineLength += 8 + maxLineLength % 2;

    StringManipulator::instance().newMenu(maxLineLength, options);
    cout << GRAY << "-> " << RESET;
    int choice;
    cin >> choice;

    if (choice == 0 || choice >= options.size()-1)
        return nullptr;

    string dbName = options[choice].substr(4);
    printActualInput(dbName);
    return CustomFormat::createDatabaseFromFile(path, dbName);

}



void sqlQuery(shared_ptr<Database> d) {
//    cin.ignore();
    QueryEditor editor = QueryEditor(d);
    editor.start();
}

void promptExport(shared_ptr<Database> database) {
    shared_ptr<Format> chosenFormat;
    StringManipulator::instance().newMenu(34, {"Choose export option", "[1] Standard SQL Export", "[2] WYL Export", "[0] Back"});
    string optStr;
    bool saved = false;
    while (true) {
        cout << "-> ";
        cin >> optStr;
        if (optStr == "1") {
            printActualInput("[1] Standard SQL Export");
            chosenFormat = make_shared<SQLFormat>(database);
            saved = true;
            break;
        } else if (optStr == "2") {
            printActualInput("[2] WYL Export");
            chosenFormat = make_shared<CustomFormat>(database);
            saved = true;
            break;
        } else if (optStr == "0") {
            printActualInput("[0] Back");
            break;
        }
    }

    if (saved)
        chosenFormat->exportDatabase();
}


void mainMenu() {
    int opt = 1;
    vector<string> opts = {"MAIN MENU", "[1] Create a new database", "[2] Load database", "[0] Exit"};
    while (!database) {
        StringManipulator::instance().newMenu(34, opts, 4);
        cout << GRAY << "-> " << RESET;
        cin >> opt;
        switch (opt) {
            case 1:
                printActualInput(opts[1]);
                database = createNewDatabaseMenu();
                break;
            case 2:
                printActualInput(opts[2]);
                database = loadDatabase();
                break;
            case 0:
                printActualInput(opts[3]);
                StringManipulator::instance().newMessageGreen("Program exited successfully.");
                return;
        }
    }
    cout << *database;


    opts = {"MAIN MENU", "[1] SQL Query", "[2] Export database", "[0] Exit"};
    bool userWantsToQuit = false;
    bool saved = true;
    while (!userWantsToQuit) {
        StringManipulator::instance().newMenu(34, opts);
        cout << GRAY << "-> " << RESET;
        cin >> opt;
        switch (opt) {
            case 1:
                printActualInput(opts[1]);
                cin.ignore();
                sqlQuery(database);
                saved = false;
                break;
            case 2: {
                printActualInput(opts[2]);
                promptExport(database);
                saved = true;
                break;
            }
            case 0:
                printActualInput(opts[3]);

                if(!saved) {
                    StringManipulator::instance().newMenu(40, {"Changes were not saved. Exit anyway?", "[1] Yes", "[2] No"});
                    string saveOpt;
                    while(!saved) {
                        cout << GRAY << "-> " << RESET;
                        cin >> saveOpt;
                        if(saveOpt == "2") {
                            printActualInput("[2] No");
                            promptExport(database);
                            saved = true;
                        } else if(saveOpt == "1") {
                            printActualInput("[1] Yes");
                            break;
                        }
                    }
                }
                printActualInput("[0] Exit");
                StringManipulator::instance().newMessageGreen("Program exited successfully.");
                userWantsToQuit = true;
        }
    }
}


//void loadDb(string path) {
//    database = CustomFormat::createDatabaseFromFile(path, "");
//}
//
//void doQuery(string query) {
//    QueryEditor qe(database);
//    qe.startQuery(query);
//}

int main() {
//    mainMenu();
//    loadDb("D:\\FAKULTET\\CetvrtiSemestar\\POOP\\DrugiProjekatCpp\\cmake-build-debug\\CustomFormatExports\\School");
//    doQuery("SELECT * FROM Student");
    return 0;


}