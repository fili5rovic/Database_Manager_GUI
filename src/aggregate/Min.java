package aggregate;

import java.util.List;

public class Min extends Aggregate {
    @Override
    protected String getName() {
        return "MIN";
    }
    @Override
    protected double aggregateOperation(List<Double> numbers) {
        double min = Double.MAX_VALUE;
        for(Double number : numbers) {
            if(number < min) {
                min = number;
            }
        }
        return min;
    }
}
