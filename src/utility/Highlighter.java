package utility;

import org.fxmisc.richtext.CodeArea;
import org.fxmisc.richtext.LineNumberFactory;
import org.fxmisc.richtext.model.StyleSpans;
import org.fxmisc.richtext.model.StyleSpansBuilder;

import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Highlighter {


    public static void initCodeArea(CodeArea codeArea, List<String> tableNames) {
        codeArea.getStyleClass().add("styled-text-area");
        codeArea.setParagraphGraphicFactory(LineNumberFactory.get(codeArea));
        codeArea.textProperty().addListener((obs, oldText, newText) -> utility.Highlighter.applyHighlighting(codeArea, tableNames));
//        utility.Highlighter.applyHighlighting(codeArea, tableNames);
    }

    private static void applyHighlighting(CodeArea codeArea, List<String> tableNames) {
        String text = codeArea.getText();
        codeArea.setStyleSpans(0, computeHighlighting(text, tableNames));
    }


    /**
     * Computes the style spans for syntax highlighting based on the text content.
     *
     * @param text The text content to highlight.
     * @return The style spans for the highlighted text.
     */
    private static StyleSpans<Collection<String>> computeHighlighting(String text, List<String> tableNames) {
        String[] patterns = {
                "(?i)\\b(?:SELECT|DISTINCT|FROM|WHERE|INSERT|INTO|VALUES|AND|OR|SHOW|TABLES|DELETE|CREATE|TABLE|DROP|UPDATE|SET|ON|JOIN|INNER|ORDER|BY|ASC|DESC|LIMIT|OFFSET)\\b",
                "\\*|=",
                "\"([^\"]*)\"",
                "'([^']*)'",
                "table_names_here"
                // obrati paznju kada dodajes novi pattern da uvek pocinje sa (?i)
        };
        String[] cssClasses = {
                "keyword",
                "star",
                "double-quotations",
                "quotations",
                "tables"
        };

        StringBuilder tableString = new StringBuilder("(?i)\\b(?:");
        for (String tableName : tableNames) {
            tableString.append(tableName).append("|");
        }
        String finalRegex = tableString.substring(0, tableString.length() - 1) + ")\\b";
        patterns[patterns.length - 1] = finalRegex;

        String combinedPattern = String.join("|", patterns);
        Pattern pattern = Pattern.compile(combinedPattern, Pattern.CASE_INSENSITIVE);
        Matcher matcher = pattern.matcher(text);
        StyleSpansBuilder<Collection<String>> spansBuilder = new StyleSpansBuilder<>();

        int lastIndex = 0;
        while (matcher.find()) {
            String styleClass = null;
            for (int i = 0; i < patterns.length; i++) {
                if (matcher.group().matches(patterns[i])) {
                    styleClass = cssClasses[i];
                    break;
                }
            }
            spansBuilder.add(Collections.singleton("default_text"), matcher.start() - lastIndex); // Regular text
            spansBuilder.add(Collections.singleton(styleClass), matcher.end() - matcher.start()); // Matched pattern
            lastIndex = matcher.end();
        }
        spansBuilder.add(Collections.singleton("default_text"), text.length() - lastIndex); // Regular text after last match
        return spansBuilder.create();
    }
}
