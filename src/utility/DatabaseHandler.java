package utility;

import db.DBManager;

import java.io.File;

public class DatabaseHandler {
    private final DBManager dbManager;
    private boolean isSaved = false;
    private String savePath = "";

    public DatabaseHandler() {
        dbManager = new DBManager();
    }


    public void saveDatabase() {
        saveDatabase(savePath);
    }
    public void saveDatabase(String savePath) {
        File file = new File(savePath);
        String fileName = file.getName();
        String filePath = file.getParent() + "\\";
        String format = "";
        int index = fileName.lastIndexOf('.');
        if (index > 0) {
            format = fileName.substring(index + 1);
        }

        dbManager.exportDatabaseCustom(filePath, fileName, format);
        System.out.println("SUCCESSFULLY SAVED: " + savePath);
        isSaved = true;
    }

    public void setIsSaved(boolean isSaved) {
        this.isSaved = isSaved;
    }

    public boolean isSaved() {
        return isSaved;
    }

    public void setSavePath(String savePath) {
        this.savePath = savePath;
    }

    public String getSavePath() {
        return savePath;
    }



}
