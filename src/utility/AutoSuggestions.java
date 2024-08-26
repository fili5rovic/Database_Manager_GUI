package utility;

import controller.DashboardController;
import javafx.geometry.Bounds;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Region;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import org.fxmisc.richtext.CodeArea;
import window.Window;

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class AutoSuggestions {

    private static MaxSizedContextMenu codeCompletionContextMenu = new MaxSizedContextMenu();
    private static int longestColumnNameSize = -1;
    private static int longestTableNameSize = -1;

    private static final int MAX_HEIGHT = 250;



    static {
        codeCompletionContextMenu.getItems().clear();
        codeCompletionContextMenu.setMaxHeight(MAX_HEIGHT);

//        EventHandler<KeyEvent> keyEventHandler = event -> {
//            if (event.getCode() == KeyCode.TAB) {
//                tabPressed();
//            }
//        };
//        codeCompletionContextMenu.addEventHandler(KeyEvent.KEY_RELEASED, keyEventHandler);

    }

    public static void checkAutocomplete(CodeArea codeArea, List<String> tableNames) {

        int currentParagraph = codeArea.getCurrentParagraph();
        String currentText = codeArea.getParagraph(currentParagraph).getText().toLowerCase();
        codeCompletionContextMenu.getItems().clear();
        codeCompletionContextMenu.hide();


        Pattern columnMatcher = Pattern.compile("(?:select|where|and|or)\\s+$|,\\s*$|(?:\\.|\\()$", Pattern.CASE_INSENSITIVE);
        Pattern tableMatcher = Pattern.compile("(?:from|join)\\s+$", Pattern.CASE_INSENSITIVE);

        Matcher fromMatcher = tableMatcher.matcher(currentText);
        Matcher selectMatcher = columnMatcher.matcher(currentText);

        if (fromMatcher.find()) {
            for (String tableName : tableNames) {
                MenuItem table = new MenuItem(tableName);

                table.setOnAction(e -> {
                    codeArea.replaceText(codeArea.getCaretPosition(), codeArea.getCaretPosition(), tableName);
                    codeCompletionContextMenu.hide();
                });
                codeCompletionContextMenu.getItems().add(table);
            }
        }
        // Check for "select" pattern
        else if (selectMatcher.find()) {
            int tableIndex = 0;

            longestColumnNameSize = getLongestColumnNameSize();
            longestTableNameSize = getLongestTableNameSize(tableNames);

            for (List<String> columnList : ((DashboardController) Window.getWindowAt(Window.WINDOW_DASHBOARD).getController()).getColumnNames()) {
                int finalTableIndex = tableIndex;

                columnList.forEach(column -> {
                    MenuItem columnItem = new MenuItem();

                    columnItem.setGraphic(createMenuItemGraphic(column, tableNames.get(finalTableIndex)));

                    columnItem.setOnAction(e -> {
                        codeArea.replaceText(codeArea.getCaretPosition(), codeArea.getCaretPosition(), column);
                        codeCompletionContextMenu.hide();
                    });

                    codeCompletionContextMenu.getItems().add(columnItem);
                });
                tableIndex++;
            }
            codeCompletionContextMenu.show(codeArea, codeArea.getLayoutX(), codeArea.getLayoutY());
        }

        // Show context menu on caret position
        try {
            Bounds caretBounds = codeArea.getCaretBounds().get();
            codeCompletionContextMenu.show(codeArea, caretBounds.getMinX(), caretBounds.getMinY() + caretBounds.getHeight());
        } catch (Exception e) {
            // Ako ne možeš da dobiješ Bounds (u slučaju da caretBounds nije dostupan)
            codeCompletionContextMenu.show(codeArea, codeArea.getLayoutX(), codeArea.getLayoutY());
        }
        codeArea.setContextMenu(codeCompletionContextMenu);


    }



    private static Node createMenuItemGraphic(String mainTextStr, String tableTextStr) {
        // Create the labels
        Label smallText = new Label("(" + tableTextStr + ")");
        smallText.setTextFill(Color.RED);
        smallText.setFont(Font.font("Arial", 11)); // Adjust font size as needed
        smallText.setStyle("-fx-text-fill: rgba(112,111,111,0.41)");
        smallText.setPrefWidth(longestTableNameSize * 8); // Adjust the width based on the longest table name size

        Label mainText = new Label(mainTextStr);
        mainText.setFont(Font.font("Arial", 11)); // Adjust font size as needed
        mainText.setPrefWidth(longestColumnNameSize * 8); // Adjust the width based on the longest column name size

        // Create an HBox and set its alignment and spacing
        HBox hbox = new HBox();
        hbox.setAlignment(Pos.CENTER_RIGHT);
        hbox.setSpacing(10); // Adjust spacing as needed
        hbox.getChildren().addAll(mainText, smallText);
        hbox.setPrefWidth(Region.USE_COMPUTED_SIZE);

        return hbox;
    }

    public static void updateSizes() {
        longestColumnNameSize = getLongestColumnNameSize();
        longestTableNameSize = getLongestTableNameSize(((DashboardController) Window.getWindowAt(Window.WINDOW_DASHBOARD).getController()).getTableNames());
    }

    private static int getLongestColumnNameSize() {
        AtomicInteger longestColumnNameSize = new AtomicInteger(-1);
        for (List<String> columnList : ((DashboardController) Window.getWindowAt(Window.WINDOW_DASHBOARD).getController()).getColumnNames()) {
            columnList.forEach(column -> {
                if (column.length() > longestColumnNameSize.get()) {
                    longestColumnNameSize.set(column.length());
                }
            });
        }
        return longestColumnNameSize.get();
    }
    private static int getLongestTableNameSize(List<String> tableNames) {
        int longestTableSize = -1;
        for(String table : tableNames) {
            if (table.length() > longestTableSize) {
                longestTableSize = table.length();
            }
        }
        return longestTableSize;
    }

}
