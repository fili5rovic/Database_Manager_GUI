#include <jni.h>
#include "db_DBManager.h"  // Generisani JNI zaglavlje
#include <string>
#include "CustomFormat.h"  // Pretpostavljam da su ovo vaše klase
#include "QueryEditor.h"
#include "SQLFormat.h"


using namespace std;

shared_ptr<Database> db = nullptr;


JNIEXPORT void JNICALL Java_db_DBManager_loadDb(JNIEnv *env, jobject obj, jstring jpath) {
    // Konvertovanje jstring u std::string
    const char *nativePath = env->GetStringUTFChars(jpath, 0);
    string path(nativePath);

    // Oslobađa memoriju koju je alocirao GetStringUTFChars
    env->ReleaseStringUTFChars(jpath, nativePath);

    // Poziv funkcije iz C++ koda
    try {
        db = CustomFormat::createDatabaseFromFile(path, "");
    } catch (exception& e) {
        cout << e.what() << endl;
    }

}

JNIEXPORT void JNICALL Java_db_DBManager_doQuery(JNIEnv *env, jobject obj, jstring jquery) {
    // Konvertovanje jstring u std::string
    const char *nativeQuery = env->GetStringUTFChars(jquery, 0);
    string query(nativeQuery);

    // Oslobađa memoriju koju je alocirao GetStringUTFChars
    env->ReleaseStringUTFChars(jquery, nativeQuery);

    // Poziv funkcije iz C++ koda
    QueryEditor qe(db);
        try {
            qe.startQuery(query);
        } catch(MyException& e) {
            cout << e.what() << endl;
        } catch (exception& e) {
            cout << "[RUNTIME_ERROR] Invalid arguments." << endl;
        }
}

JNIEXPORT void JNICALL Java_db_DBManager_outputTableNames(JNIEnv *, jobject) {
    if (db == nullptr) {
        cout << "[RUNTIME_ERROR] Database not loaded." << endl;
        return;
    }
    ofstream outFile("output.txt", ios::out);
    for (const auto& tableName : db->getAllTableNames()) {
        outFile << tableName << "|";
        for( const auto& column : db->tryGettingTableByNameCaseI(tableName)->getTableHeaders()) {
            outFile << column << ",";
        }
        outFile << endl;
    }
    outFile.close();
}

JNIEXPORT void JNICALL Java_db_DBManager_exportDatabaseCustom
  (JNIEnv* env, jobject, jstring jPath, jstring jDbName, jstring jFormat) {
    const char* pathCStr = env->GetStringUTFChars(jPath, nullptr);
    const char* dbNameCStr = env->GetStringUTFChars(jDbName, nullptr);
    const char* formatCStr = env->GetStringUTFChars(jFormat, nullptr);

    std::string path(pathCStr);
    std::string dbName(dbNameCStr);
    std::string format(formatCStr);

    // Pozivamo funkciju za eksportovanje baze podataka
    if (db == nullptr) {
        std::cout << "[RUNTIME_ERROR] Database not loaded." << std::endl;
    } else if (format == "wyl") {
        CustomFormat cf(db);
        cf.finalExport(path, dbName);
    } else if (format == "sql") {
        SQLFormat sf(db);
        sf.finalExport(path, dbName);
    } else {
        std::cout << "[RUNTIME_ERROR] Invalid format." << std::endl;
    }

    // Oslobađamo memoriju
    env->ReleaseStringUTFChars(jPath, pathCStr);
    env->ReleaseStringUTFChars(jDbName, dbNameCStr);
    env->ReleaseStringUTFChars(jFormat, formatCStr);
}