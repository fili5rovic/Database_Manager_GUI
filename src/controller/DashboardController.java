package controller;

import aggregate.Aggregate;
import db.DBManager;
import utility.*;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import org.fxmisc.richtext.CodeArea;
import org.fxmisc.richtext.model.PlainTextChange;
import window.Window;
import window.WindowHelper;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URL;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * The DashboardController class is responsible for managing the user interface and functionality
 * of the main dashboard window in the application.
 */
public class DashboardController extends ControllerBase {

    /**
     * The main split pane.
     */
    @FXML
    private SplitPane mainSplitPane;
    /**
     * The split pane for the side panel.
     */
    @FXML
    private SplitPane sideSplitPane;
    /**
     * The code area for the SQL code.
     */
    @FXML
    private CodeArea codeAreaRichFX;
    /**
     * The tab pane for the output tables.
     */
    @FXML
    private TabPane tabPane;
    /**
     * The button for closing the console.
     */
    @FXML
    private Button closeConsoleBtn;
    /**
     * The button for maximizing the console.
     */
    @FXML
    private Button maximizeConsoleBtn;
    /**
     * The button to run code
     */
    @FXML
    private Button runBtn;
    /**
     * The button for showing the selected table.
     */
    @FXML
    private Button showBtn;
    /**
     * The button for dropping the selected table.
     */
    @FXML
    private Button dropBtn;
    /**
     * The tree view for the database tables.
     */
    @FXML
    private TreeView<String> treeView;
    /**
     * The radio menu item for the light theme.
     */
    @FXML
    private RadioMenuItem lightThemeBtn;
    /**
     * The radio menu item for the dark theme.
     */
    @FXML
    private RadioMenuItem darkThemeBtn;
    /**
     * Used for saving file content for parsing
     */
    private String outputString;
    /**
     * Used for saving error file content for parsing
     */
    private String errorString;
    /**
     * The database manager for handling database operations.
     */
    private DBManager dbManager;

    private DatabaseHandler databaseHandler;
    private UIHandler uiHandler;

    private boolean distinct;
    private int aggregateNumber;

    private int limit = 0;
    private int offset = 0;
    private final List<String> orderByStrings = new ArrayList<>();

    private final List<Aggregate> aggregateList = new ArrayList<>();

    /**
     * Initializes the controller class. This method is automatically called after the FXML file has been loaded.
     *
     * @param location  The location used to resolve relative paths for the root object, or null if the location is not known.
     * @param resources The resources used to localize the root object, or null if the root object was not localized.
     */
    @Override
    public void initialize(URL location, ResourceBundle resources) {
        Window.getWindowAt(Window.WINDOW_DASHBOARD).setController(this);
        setCssClasses();
        listeners();
        dbManager = new DBManager();
        databaseHandler = new DatabaseHandler();
        uiHandler = new UIHandler(mainSplitPane, tabPane, treeView);
        String path = System.getProperty("user.dir") + "\\src\\sample\\empty";
        loadDatabase(path);
        utility.Highlighter.initCodeArea(codeAreaRichFX, uiHandler.getTableNames()); // nikad ne zovi pre loadDatabase, nece se bojiti lepo nazivi tabela
        selectCorrectThemeItem();
        onTextChanged(null);
    }

    /**
     * Initializes CSS classes for certain components. Should be called at the start of program.
     */
    private void setCssClasses() {
        runBtn.getStyleClass().add("button-light-green");
        maximizeConsoleBtn.getStyleClass().add("button-light-yellow");
        closeConsoleBtn.getStyleClass().add("button-light-red");
    }

    /**
     * A method for selecting the current theme. Reads a file default-theme.txt to check which theme was last applied.
     */
    private void selectCorrectThemeItem() {
        String theme = FileHelper.readFromFile(System.getProperty("user.dir") + "/src/default_theme/default-theme.txt");
        darkThemeBtn.setSelected(theme.equals("dark_theme.css"));
        lightThemeBtn.setSelected(theme.equals("light_theme.css"));
    }

    /**
     * Handles the action for the run button, executing the SQL queries from the CodeArea.
     */
    public void runButton() {
        String queries;
        if(codeAreaRichFX.getSelectedText().isEmpty())
            queries = codeAreaRichFX.getText();
        else
            queries = codeAreaRichFX.getSelectedText();

        for (String query : queries.split(";")) {
            query = query.replace('\n', ' ').trim();
            if (query.isEmpty())
                continue;

            String oldQuery = query;
            query = query.replaceAll("(?i)SELECT\\s+DISTINCT\\s+", "select ");
            distinct = !oldQuery.equals(query);

            boolean hadError = checkForAggregates(query);
            if(hadError) {
                System.out.println("ERROR");
                uiHandler.addErrorTab(errorString);
                this.errorString = FileHelper.readFromFile("error.txt");
                updateOutputTables();
                return;
            }

            if(query.toLowerCase().contains("select")) {
                checkForLimit(query);
                if (query.toLowerCase().contains("limit"))
                    query = query.toLowerCase().split("limit")[0].trim();
                if(query.toLowerCase().contains("offset"))
                    query = query.toLowerCase().split("offset")[0].trim();

                checkForOrderBy(query);
                if(query.toLowerCase().contains("order"))
                    query = query.toLowerCase().split("order")[0].trim();
                if(query.toLowerCase().contains("by"))
                    query = query.toLowerCase().split("by")[0].trim();

                query = query.replaceAll(Aggregate.getPattern().toString(), "$1");
            }

            dbManager.doQuery(query);
            tableOutput();
            if (query.toLowerCase().contains("create") || query.toLowerCase().contains("drop")) {
                dbManager.outputTableNames();
                populateTreeView();
            }
            if (databaseHandler.isSaved())
                databaseHandler.setIsSaved(query.toLowerCase().contains("select") || query.toLowerCase().contains("show"));
        }
    }

    private void checkForOrderBy(String query) {
        orderByStrings.clear();
        query = query.toLowerCase().replace("order", "");
        if(!query.toLowerCase().contains("by"))
            return;
        String orderByStr = query.toLowerCase().split("by")[1];
        String[] orderBy = orderByStr.split(",");
        for(int i = 0; i < orderBy.length; i++) {
            orderBy[i] = orderBy[i].trim();
            orderByStrings.add(orderBy[i]);
        }
    }

    private void checkForLimit(String query) {
        limit = 0;
        offset = 0;
        if(!query.toLowerCase().contains("limit"))
            return;
        String limitStr = query.toLowerCase().split("limit")[1];
        if(limitStr.contains("offset")) {
            String[] limitOffset = limitStr.split("offset");
            limit = Integer.parseInt(limitOffset[0].trim());
            offset = Integer.parseInt(limitOffset[1].trim());
        } else
            limit = Integer.parseInt(limitStr.trim());

        if(limit < 0)
            limit = 0;
    }

    private boolean checkForAggregates(String query) {
        aggregateNumber = 0;
        aggregateList.clear();

        Pattern pattern = Aggregate.getPattern();
        Matcher matcher = pattern.matcher(query);
        if(matcher.find()) {
            aggregateNumber = query.split(",").length;
            for(int i = 0; i < aggregateNumber; i++) {
                String currentParam = query.split(",")[i];
                if(currentParam.toLowerCase().contains("sum")) {
                    aggregateList.add(new aggregate.Sum());
                } else if(currentParam.toLowerCase().contains("avg")) {
                    aggregateList.add(new aggregate.Avg());
                } else if(currentParam.toLowerCase().contains("count")) {
                    System.out.println("COUNT");
                    aggregateList.add(new aggregate.Count());
                } else if(currentParam.toLowerCase().contains("min")) {
                    aggregateList.add(new aggregate.Min());
                } else if(currentParam.toLowerCase().contains("max")) {
                    aggregateList.add(new aggregate.Max());
                } else {
                    FileHelper.writeToFile("error.txt", "[RUNTIME_ERROR] Different number of rows in table!");
                    aggregateNumber = i;
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Updates the output tables by reading from the output and error files.
     */
    public void tableOutput() {
        if(this.aggregateNumber > 0) {
            StringBuilder finalOutput = new StringBuilder();
            String[] aggregateStrings = new String[this.aggregateNumber];
            for(int i = 0; i < this.aggregateNumber; i++) {
                aggregateStrings[i] = aggregateList.get(i).executeAggregate("output.txt", i);
            }
            finalOutput.append(aggregateStrings[0].split("\n")[0]).append("\n");
            for(int i = 0; i < this.aggregateNumber; i++) {
                finalOutput.append(aggregateStrings[i].split("\n")[1]);
            }
            finalOutput.append("\n");
            for(int i = 0; i < this.aggregateNumber; i++) {
                finalOutput.append(aggregateStrings[i].split("\n")[2]);
            }
            System.out.println(finalOutput);
            FileHelper.writeToFile("output.txt", finalOutput.toString());
            this.aggregateNumber = 0;
        }

        if(!orderByStrings.isEmpty())
            utility.OrderBy.writeOrderedToFile("output.txt", orderByStrings);

        if(limit > 0 || offset > 0)
            FileHelper.limitFileLines(limit, offset,"output.txt");

        if(distinct)
            this.outputString = FileHelper.readFromFileNoDuplicates("output.txt");
        else
            this.outputString = FileHelper.readFromFile("output.txt");

        this.errorString = FileHelper.readFromFile("error.txt");
        orderByStrings.clear();
        limit = 0;
        distinct = false;
        updateOutputTables();
    }

    /**
     * Handles the action for the New Database menu item.
     */
    public void newDbMenuItem() {
        Stage stage = new Stage();
        WindowEvent event = new WindowEvent(stage, WindowEvent.WINDOW_SHOWN);
        onAppClose(event);
        if (event.isConsumed())
            return;

        String path = System.getProperty("user.dir") + "\\src\\sample\\empty";
        databaseHandler.setSavePath("");
        loadDatabase(path);
        deleteSQLQuery();
    }

    @FXML
    private void loadDbMenuItem() {
        loadDbMenuItem(Window.getWindowAt(Window.WINDOW_DASHBOARD).getStage());
    }

    /**
     * Handles the action for the Load Database menu item.
     *
     * @param currStage The current stage.
     */
    public void loadDbMenuItem(Stage currStage) {
        Stage stage = new Stage();
        WindowEvent event = new WindowEvent(stage, WindowEvent.WINDOW_SHOWN);
        onAppClose(event);
        if (event.isConsumed())
            return;

        String path = System.getProperty("user.dir");
        File selectedFile = FileHelper.selectDatabaseFile(path, currStage);
        if (selectedFile == null)
            return;
        String selectedPath = selectedFile.getAbsolutePath();
//        System.out.println("SAVEPATH: " + selectedPath);
        databaseHandler.setSavePath(selectedPath);
        if (selectedPath.endsWith("sql")) {
            String sqlString = FileHelper.readFromFile(selectedPath);
            String wylString = FileHelper.convertSQLToWYL(sqlString);
            String wylPath = System.getProperty("user.dir") + "/tempFile.wyl";
            try (FileWriter fileWriter = new FileWriter(wylPath)) {
                fileWriter.write(wylString);
            } catch (IOException e) {
                System.out.println("Couldn't load file!");
            }
            String finalPath = wylPath.substring(0, wylPath.length() - 4);
            loadDatabase(finalPath);
            deleteSQLQuery();

            File file = new File(wylPath);
            if (!file.delete()) {
                System.out.println("Unable to delete file");
            }
        } else {
            String finalPath = selectedPath.substring(0, selectedPath.length() - 4);
            loadDatabase(finalPath);
            deleteSQLQuery();
        }


    }

    /**
     * Loads the database from the specified path.
     *
     * @param path The path to the database file.
     */
    private void loadDatabase(String path) {
        dbManager.loadDb(path);
        dbManager.outputTableNames();
        populateTreeView();
        databaseHandler.setIsSaved(true);
    }

    /**
     * Updates the output tables based on the output string.
     */
    private void updateOutputTables() {
        uiHandler.updateOutputTables(outputString, errorString);
    }


    /**
     * Populates the TreeView with the database table names.
     */
    private void populateTreeView() {
        uiHandler.populateTreeView();
    }

    /**
     * Adds listeners to the UI components.
     */
    private void listeners() {
        sideSplitPane.getDividers().forEach(divider -> divider.positionProperty().addListener((observable, oldValue, newValue) -> {
            double minPosition = 0.1321;
            double maxPosition = 0.4;

            if (newValue.doubleValue() < minPosition) {
                divider.setPosition(minPosition);
            }

            if (newValue.doubleValue() > maxPosition) {
                divider.setPosition(maxPosition);
            }
        }));

        codeAreaRichFX.plainTextChanges().subscribe(this::onTextChanged);


        treeView.getSelectionModel().selectedItemProperty().addListener((observable, oldValue, newValue) -> {
            if (newValue != null && !newValue.isLeaf()) {
                showBtn.setDisable(true);
                dropBtn.setDisable(true);
            } else {
                showBtn.setDisable(false);
                dropBtn.setDisable(false);
            }
        });
    }

    private void onTextChanged(PlainTextChange plainTextChange) {
        runBtn.setDisable(codeAreaRichFX.getText().trim().isEmpty());

        AutoSuggestions.checkAutocomplete(codeAreaRichFX, uiHandler.getTableNames());
    }

    /**
     * Button action that maximizes the console.
     */
    @FXML
    private void maximizeConsole() {
        uiHandler.maximizeConsole();
    }

    /**
     * Button action that closes the console.
     */
    @FXML
    private void closeConsole() {
        uiHandler.closeConsole();
    }

    /**
     * Selects the light theme.
     */
    @FXML
    private void lightThemeBtn() {
        utility.ThemeSelector.setTheme("light_theme", Window.WINDOW_DASHBOARD);
    }

    /**
     * Selects the dark theme.
     */
    @FXML
    private void darkThemeBtn() {
        utility.ThemeSelector.setTheme("dark_theme", Window.WINDOW_DASHBOARD);
    }

    /**
     * Deletes content from SQL code area.
     */
    @FXML
    private void deleteSQLQuery() {
        codeAreaRichFX.clear();
    }

    /**
     * Gets called upon saving as .sql database.
     */
    @FXML
    private void saveSqlBtn() {
        WindowHelper.showWindow(Window.WINDOW_SAVE_AS);
        SaveAsController saveAsController = (SaveAsController) Window.getWindowAt(Window.WINDOW_SAVE_AS).getController();
        saveAsController.toggleBtn("sql");
    }

    /**
     * Gets called upon saving as .wyl database.
     */
    @FXML
    private void saveCustomFormatBtn() {
        uiHandler.saveDatabaseCustomFormatPopup();
    }

    /**
     * Button action for dropping selected table in TreeView.
     */
    @FXML
    private void dropSelectedTable() {
        if (treeView.getSelectionModel().getSelectedItem() == null || !treeView.getSelectionModel().getSelectedItem().isLeaf())
            return;

        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Drop Table Confirmation");
        alert.setHeaderText("Are you sure you want to drop the table?");

        Optional<ButtonType> result = alert.showAndWait();
        if (result.isPresent() && result.get() == ButtonType.OK) {
            String currSelected = treeView.getSelectionModel().getSelectedItem().getValue();
            dbManager.doQuery("DROP TABLE " + currSelected);
            databaseHandler.setIsSaved(false);
            dbManager.outputTableNames();
            populateTreeView();
            treeView.getSelectionModel().clearSelection();
            deleteSQLQuery();
        }
    }

    /**
     * Button action for showing selected table in TreeView.
     */
    @FXML
    private void showSelectedTableBtn() {
        if (treeView.getSelectionModel().getSelectedItem() == null || !treeView.getSelectionModel().getSelectedItem().isLeaf()) {
            dropBtn.setDisable(true);
            return;
        }
        String currSelected = treeView.getSelectionModel().getSelectedItem().getValue();
        dbManager.doQuery("SELECT * FROM " + currSelected);
        tableOutput();
    }

    /**
     * Method gets called when "SAVE" menu is selected.
     */
    @FXML
    private void saveMenuBtn() {
        if (databaseHandler.getSavePath().isEmpty()) {
            saveCustomFormatBtn();
        } else {
            databaseHandler.saveDatabase();
        }
    }

    /**
     * Method gets called on app closing. Checks if user wants to quit with unsaved work.
     *
     * @param e {@link WindowEvent} object.
     */
    public void onAppClose(WindowEvent e) {
        if (databaseHandler.isSaved())
            return;

        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Changes may be unsaved");
        alert.setHeaderText("Unsaved changes");

        String theme = "../" + FileHelper.readFromFile("src/default_theme/default-theme.txt");
        alert.getDialogPane().getStylesheets().add(Objects.requireNonNull(getClass().getResource(theme)).toExternalForm());

        alert.setContentText("Do you want to save your changes before exiting?");

        ButtonType saveButton = new ButtonType("Save");
        ButtonType discardButton = new ButtonType("Discard");
        ButtonType cancelButton = new ButtonType("Cancel", ButtonBar.ButtonData.CANCEL_CLOSE);
        alert.getButtonTypes().setAll(saveButton, discardButton, cancelButton);


        Optional<ButtonType> result = alert.showAndWait();

        if (result.isPresent()) {
            if (result.get() == saveButton) {
                saveMenuBtn();
                WindowHelper.closeAllWindows();
            } else if (result.get() == discardButton) {
                if (e.getEventType() == WindowEvent.WINDOW_CLOSE_REQUEST) {
                    WindowHelper.closeAllWindows();
                }
            } else {
                e.consume();
            }
        }
    }


    /**
     * Sets save path.
     *
     * @param path Path to save.
     */
    public void setSavePath(String path) {
        databaseHandler.setSavePath(path);
        databaseHandler.setIsSaved(true);
    }

    /**
     * Datbase manager getter.
     *
     * @return {@link DBManager} object.
     */
    public DBManager getDbManager() {
        return dbManager;
    }

    public List<String> getTableNames() {
        return uiHandler.getTableNames();
    }

    public List<List<String>> getColumnNames() {
        return uiHandler.getColumnNames();
    }

    public void undoCodeArea() {
        codeAreaRichFX.undo();
    }
    public void redoCodeArea() {
        codeAreaRichFX.redo();
    }
}
