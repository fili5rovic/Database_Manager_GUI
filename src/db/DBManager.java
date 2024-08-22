package db;

public class DBManager {

//    static {                              // OTKOMENTARSII OVO AKO NE RADI
//        System.loadLibrary("native");
//    }

    public native void loadDb(String name);
    public native void doQuery(String query);

    public native void outputTableNames();

    public native void exportDatabaseCustom(String path, String dbName, String format);

    public static void main(String[] args) {
        DBManager dbManager = new DBManager();
        dbManager.loadDb("D:\\FAKULTET\\CetvrtiSemestar\\POOP\\DrugiProjekatCpp\\cmake-build-debug\\CustomFormatExports\\School");
//        dbManager.getAllTableNamesIntoOutputFile();
        dbManager.doQuery("SELECT * FROM Student");
    }

}
