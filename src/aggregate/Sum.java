package aggregate;

import java.util.List;

public class Sum extends Aggregate {

    @Override
    protected double aggregateOperation(List<Double> numbers) {
        double sum = 0;
        for (Double number : numbers) {
            sum += number;
        }
        return sum;
    }

    @Override
    protected String getName() {
        return "SUM";
    }


}
