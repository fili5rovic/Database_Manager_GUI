package window;

import utility.FileHelper;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;

public class WelcomeWindow extends Window{
    @Override
    public void init(Stage stage) {
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("../sample/welcome.fxml"));
            Scene scene = new Scene(fxmlLoader.load());
            String theme = "../" + FileHelper.readFromFile("src/default_theme/default-theme.txt");
            scene.getStylesheets().add(getClass().getResource(theme).toExternalForm());
            stage.setResizable(false);
            stage.setTitle("Load Database");
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
