package window;

import controller.DashboardController;
import utility.FileHelper;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;

public class DashboardWindow extends Window {
    @Override
    public void init(Stage stage) {
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("../sample/dashboard01.fxml"));
            Scene scene = new Scene(fxmlLoader.load(), 1200, 800);
            String theme = "../" + FileHelper.readFromFile("src/default_theme/default-theme.txt");
            scene.getStylesheets().add(getClass().getResource(theme).toExternalForm());
            stage.setResizable(false);
            stage.setTitle("Database Manager");
            stage.setScene(scene);

            String iconPath = System.getProperty("user.dir") + "/pictures/icon.png";
            Image icon = new Image("file:" + iconPath);
            stage.getIcons().add(icon);

            this.stage = stage;

            this.stage.setOnCloseRequest(e-> {
                ((DashboardController) Window.getWindowAt(Window.WINDOW_DASHBOARD).getController()).onAppClose(e);
            });


        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}