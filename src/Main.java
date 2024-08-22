import javafx.application.Application;
import javafx.stage.Stage;
import window.*;

public class Main extends Application {

    static {
        System.loadLibrary("native");
    }

    @Override
    public void start(Stage primaryStage) {
        Window.setWindowAt(Window.WINDOW_WELCOME, new WelcomeWindow());
        Window.setWindowAt(Window.WINDOW_DASHBOARD, new DashboardWindow());
        Window.setWindowAt(Window.WINDOW_SAVE_AS, new SaveAsWindow());
        Window.initAllWindows();
        WindowHelper.showOnly(Window.WINDOW_WELCOME);
    }

    public static void main(String[] args) {
        launch(args);
    }
}
