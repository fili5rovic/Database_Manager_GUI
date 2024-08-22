package controller;

import db.DBManager;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.FileChooser;
import window.Window;
import window.WindowHelper;

import java.io.File;
import java.net.URL;
import java.util.ResourceBundle;

public class SaveAsController extends ControllerBase {

    @FXML
    private TextField filePathField;
    @FXML
    private ToggleGroup exportGroup;
    @FXML
    private RadioButton sqlRadioButton;
    @FXML
    private RadioButton customRadioButton;

    @FXML
    private Button saveButton;

    private String fileName;

    private File defaultDirectory;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        Window.getWindowAt(Window.WINDOW_SAVE_AS).setController(this);

        String formatString = getSelectedExportString();
        File defaultDirectory;
        if(formatString.equals("sql")) {
            defaultDirectory = new File(System.getProperty("user.dir") + "/SQLFormatExports");
        } else {
            defaultDirectory = new File(System.getProperty("user.dir") + "/CustomFormatExports");
        }
        filePathField.setText(defaultDirectory.getAbsolutePath());
    }

    public void toggleBtn(String buttonString) {
        if(buttonString.equals("sql"))
            exportGroup.selectToggle(sqlRadioButton);
        else if(buttonString.equals("custom"))
            exportGroup.selectToggle(customRadioButton);
        else System.out.println("Invalid button selected");
    }

    private void setDefaultDirectoryText(String formatString) {
        if(formatString.equals("sql")) {
            defaultDirectory = new File(System.getProperty("user.dir") + "/SQLFormatExports");
        } else {
            defaultDirectory = new File(System.getProperty("user.dir") + "/CustomFormatExports");
        }
        filePathField.setText(defaultDirectory.getAbsolutePath());
    }

    public void browseBtn() {
        FileChooser fileChooser = new FileChooser();

        String formatString = getSelectedExportString();
        fileChooser.getExtensionFilters().add(new FileChooser.ExtensionFilter(formatString.toUpperCase() +" Files",  "*." + formatString));

        setDefaultDirectoryText(formatString);


        fileChooser.setInitialDirectory(defaultDirectory);
        // Prikazi dijalog za izbor fajla
        File file = fileChooser.showSaveDialog(null);
        if(file != null) {
            filePathField.setText(file.getAbsolutePath().substring(0, file.getAbsolutePath().length()- file.getName().length()));
            this.fileName = file.getName();
            saveButton.setDisable(false);
        } else {
            saveButton.setDisable(true);
        }
    }

    public void cancelExportBtn() {
        WindowHelper.hideWindow(Window.WINDOW_SAVE_AS);
    }

    public void completeExportBtn() {
        String format = getSelectedExportString();
        getDatabaseManager().exportDatabaseCustom(filePathField.getText(), fileName, format);
        String savePath = filePathField.getText() + "\\" + fileName;
        System.out.println("SAVE PATH: " + savePath);
        ((DashboardController) Window.getWindowAt(Window.WINDOW_DASHBOARD).getController()).setSavePath(savePath);
        WindowHelper.hideWindow(Window.WINDOW_SAVE_AS);
    }

    private String getSelectedExportString() {
        Toggle selectedToggle = exportGroup.getSelectedToggle();
        RadioButton selectedRadioButton = (RadioButton) selectedToggle;
        return selectedRadioButton.getText().toLowerCase();
    }

    private DBManager getDatabaseManager() {
       return ((DashboardController) Window.getWindowAt(Window.WINDOW_DASHBOARD).getController()).getDbManager();
    }

    public void sqlSaveSelected() {
        setDefaultDirectoryText("sql");
    }

    public void wylSaveSelected() {
        setDefaultDirectoryText("wyl");
    }
}
