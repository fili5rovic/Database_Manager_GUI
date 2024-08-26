package aggregate;

import java.util.List;

public class Avg extends Aggregate {
    @Override
    protected String getName() {
        return "AVG";
    }

    @Override
    protected double aggregateOperation(List<Double> numbers) {
        double sum = 0;
        int cnt = 0;
        for(Double number : numbers) {
            cnt++;
            sum += number;
        }
        return sum / cnt;
    }
}
