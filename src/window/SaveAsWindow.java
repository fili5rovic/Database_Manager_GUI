package window;

import utility.FileHelper;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class SaveAsWindow extends Window {
    @Override
    public void init(Stage stage) {
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("../sample/saveAs.fxml"));
            Scene scene = new Scene(fxmlLoader.load(), 330, 200);
            String theme = "../" + FileHelper.readFromFile("src/default_theme/default-theme.txt");
            scene.getStylesheets().add(getClass().getResource(theme).toExternalForm());
            stage.setResizable(true);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.setTitle("Save As");
            stage.setScene(scene);

            String iconPath = System.getProperty("user.dir") + "/pictures/icon.png";
            Image icon = new Image("file:" + iconPath);
            stage.getIcons().add(icon);

            this.stage = stage;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}