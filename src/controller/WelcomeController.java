package controller;

import javafx.fxml.FXML;
import window.Window;
import window.WindowHelper;

import java.net.URL;
import java.util.ResourceBundle;

public class WelcomeController extends ControllerBase {

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        Window.getWindowAt(Window.WINDOW_DASHBOARD).setController(this);
    }

    @FXML
    private void newDbBtn() {
        DashboardController controller = (DashboardController) Window.getWindowAt(Window.WINDOW_DASHBOARD).getController();
        controller.newDbMenuItem();
        WindowHelper.showOnly(Window.WINDOW_DASHBOARD);
    }

    @FXML
    private void loadDbBtn() {
        DashboardController controller = (DashboardController) Window.getWindowAt(Window.WINDOW_DASHBOARD).getController();
        controller.loadDbMenuItem(Window.getWindowAt(Window.WINDOW_WELCOME).getStage());
        WindowHelper.showOnly(Window.WINDOW_DASHBOARD);
    }
}
