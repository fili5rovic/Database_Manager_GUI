package utility;

import controller.SaveAsController;
import javafx.scene.control.*;
import javafx.scene.layout.AnchorPane;
import javafx.util.Duration;
import window.Window;
import window.WindowHelper;

import java.util.ArrayList;
import java.util.List;

public class UIHandler implements IUIManager {

    private final SplitPane mainSplitPane;
    private final TabPane tabPane;
    private final TreeView<String> treeView;
    private final List<String> tableNames = new ArrayList<>();
    private final List<List<String>> columnNames = new ArrayList<>();

    public UIHandler(SplitPane mainSplitPane, TabPane tabPane, TreeView<String> treeView) {
        this.mainSplitPane = mainSplitPane;
        this.tabPane = tabPane;
        this.treeView = treeView;
    }


    public void saveDatabaseCustomFormatPopup() {
        WindowHelper.showWindow(Window.WINDOW_SAVE_AS);
        SaveAsController saveAsController = (SaveAsController) Window.getWindowAt(Window.WINDOW_SAVE_AS).getController();
        saveAsController.toggleBtn("custom");
    }

    /**
     * Populates the TreeView with the database table names.
     */
    public void populateTreeView() {
        treeView.setRoot(null);
        TreeItem<String> root = new TreeItem<>();
        root.setExpanded(true);

        TreeItem<String> tablesItem = new TreeItem<>("Tables");
        tablesItem.setExpanded(true);

        String[] fileContent = FileHelper.readFromFile("output.txt").split("\n");

        tableNames.clear();
        columnNames.clear();
        for (String content : fileContent) {
            if (content.trim().isEmpty())
                continue;
            ArrayList<String> columns = new ArrayList<>();
            for (String column : content.split("\\|")[1].split(",")) {
                if (!column.trim().isEmpty())
                    columns.add(column);
            }
            columnNames.add(columns);

            String name = content.split("\\|")[0];
            TreeItem<String> item = new TreeItem<>(name);
            tablesItem.getChildren().add(item);
            tableNames.add(name);
        }
        AutoSuggestions.updateSizes();

        root.getChildren().add(tablesItem);
        treeView.setRoot(root);
    }


    /**
     * Updates the output tables based on the output string.
     */
    public void updateOutputTables(String outputString, String errorString) {
        if (!errorString.isEmpty()) {
            addErrorTab(errorString);
            if (mainSplitPane.getDividers().get(0).getPosition() > 0.8)
                mainSplitPane.setDividerPosition(0, 0.8);
            return;
        }
        tabPane.getTabs().clear();


        String[] tabSeperatedString = outputString.split("@");
        for (int i = 0; i < tabSeperatedString.length; i++) {
            tabSeperatedString[i] = tabSeperatedString[i].trim();
            Tab tab = new Tab("RESULT " + (i + 1));

            TableView<String[]> currTableView = new TableView<>();
            currTableView.setEditable(false);

            currTableView.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
            currTableView.getStyleClass().add("table-view");

            AnchorPane anchorPane = new AnchorPane(currTableView);
            AnchorPane.setTopAnchor(currTableView, 0.0);
            AnchorPane.setBottomAnchor(currTableView, 0.0);
            AnchorPane.setLeftAnchor(currTableView, 0.0);
            AnchorPane.setRightAnchor(currTableView, 0.0);


            tab.setContent(anchorPane);
            tab.setOnCloseRequest(e -> {
                if (tabPane.getTabs().size() == 1)
                    closeConsole();
            });
            tabPane.getTabs().add(tab);
            tabPane.getSelectionModel().selectLast();

            updateSingleOutputTable(currTableView, tabSeperatedString[i]);

        }
    }

    /**
     * Updates single output table based on the output string.
     */
    private void updateSingleOutputTable(TableView<String[]> table, String textString) {
        table.getItems().clear();
        table.getColumns().clear();

        String[] textArr = textString.split("\\n");
        if (textArr.length < 2)
            return;

        String tableName = textArr[0];
        tabPane.getSelectionModel().getSelectedItem().setText(" " + tableName + " ");
        String[] header = textArr[1].split("\\|");
        for (String h : header) {
            TableColumn<String[], String> tableColumn = new TableColumn<>(h);
            final int columnIndex = table.getColumns().size();

            tableColumn.setCellValueFactory(data -> javafx.beans.binding.Bindings.createObjectBinding(() -> {
                String[] rowData = data.getValue();
                if(rowData.length <= columnIndex)
                    return "";
                return rowData[columnIndex];
            }));

            table.getColumns().add(tableColumn);
        }

        for (int i = 2; i < textArr.length; i++) {
            String[] data = textArr[i].split("\\|");
            table.getItems().add(data);
        }

        double goodDividerPosition = 0.961 - table.getItems().size() * 0.032467;
        if (mainSplitPane.getDividers().get(0).getPosition() > goodDividerPosition) {
            Animator.animateDividerPosition(mainSplitPane, goodDividerPosition, Duration.millis(30), tabPane::requestFocus);
        }
    }

    public void addErrorTab(String errorString) {
        Tab tab = new Tab("ERROR");
        TextArea errorTextArea = new TextArea(errorString);
        errorTextArea.setEditable(false);
        errorTextArea.getStyleClass().add("error-area");


        AnchorPane anchorPane = new AnchorPane(errorTextArea);
        AnchorPane.setTopAnchor(errorTextArea, 0.0);
        AnchorPane.setBottomAnchor(errorTextArea, 0.0);
        AnchorPane.setLeftAnchor(errorTextArea, 0.0);
        AnchorPane.setRightAnchor(errorTextArea, 0.0);

        tab.setContent(anchorPane);
        tabPane.getTabs().clear();
        tabPane.getTabs().add(tab);
    }

    public void maximizeConsole() {
        Animator.animateDividerPosition(mainSplitPane, 0.0, Duration.millis(200), tabPane::requestFocus);
    }

    public void closeConsole() {
        Animator.animateDividerPosition(mainSplitPane, 1.0, Duration.millis(200), tabPane::requestFocus);
    }

    public List<String> getTableNames() {
        return tableNames;
    }

    public List<List<String>> getColumnNames() {
        return columnNames;
    }

}
