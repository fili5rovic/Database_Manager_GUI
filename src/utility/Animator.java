package utility;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.scene.control.SplitPane;
import javafx.util.Duration;

public class Animator {
    public static void animateDividerPosition(SplitPane splitPane, double endPosition, Duration duration, Runnable onFinished) {
        double startPosition = splitPane.getDividers().get(0).getPosition();
        Timeline timeline = new Timeline();
        final int frames = 30;
        final double step = (endPosition - startPosition) / frames;

        for (int i = 0; i <= frames; i++) {
            final double position = startPosition + step * i;
            KeyFrame keyFrame = new KeyFrame(Duration.millis((duration.toMillis() / frames) * i), event -> {
                splitPane.setDividerPosition(0, position);
            });
            timeline.getKeyFrames().add(keyFrame);
        }

        timeline.setOnFinished(event -> {
            splitPane.setDividerPosition(0, endPosition);
            onFinished.run();
        });

        timeline.play();
    }
}
