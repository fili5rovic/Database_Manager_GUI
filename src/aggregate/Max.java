package aggregate;

import java.util.List;

public class Max extends Aggregate {
    @Override
    protected String getName() {
        return "MAX";
    }

    @Override
    protected double aggregateOperation(List<Double> numbers) {
        double max = Double.MIN_VALUE;
        for(Double number : numbers) {
            if(number > max) {
                max = number;
            }
        }
        return max;
    }
}
