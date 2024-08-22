package utility;

import window.Window;

import java.io.FileWriter;
import java.io.IOException;

public class ThemeSelector {

    public static void setTheme(String theme, int windowIndex) {
        String themeNameWithExtension = theme + ".css";
        Window.getWindowAt(windowIndex).getStage().getScene().getStylesheets().clear();
        Window.getWindowAt(windowIndex).getStage().getScene().getStylesheets().add(ThemeSelector.class.getResource("../" + themeNameWithExtension).toExternalForm());
        try (FileWriter fileWriter = new FileWriter(System.getProperty("user.dir") + "/src/default_theme/default-theme.txt")) {
            fileWriter.write(themeNameWithExtension);
        } catch (IOException e) {
            System.out.println("Couldn't load file!");
        }
    }
}
