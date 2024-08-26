package aggregate;

import java.util.List;

public class Count extends Aggregate {
    @Override
    protected String getName() {
        return "COUNT";
    }

    @Override
    protected double aggregateOperation(List<Double> numbers) {
        return numbers.size();
    }
}
