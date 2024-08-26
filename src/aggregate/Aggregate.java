package aggregate;

import utility.FileHelper;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

public abstract class Aggregate {


    protected abstract String getName();

    protected abstract double aggregateOperation(List<Double> numbers);

    public String executeAggregate(String path, int index) {
        String content = FileHelper.readFromFile(path);
        StringBuilder retString = new StringBuilder();
        List<Double> numbers = new ArrayList<>();
        for(int i = 0; i < content.split("\n").length; i++) {
            String line = content.split("\n")[i];
            if(i < 2) {
                if(i == 1) {
                    String[] headers = line.split("\\|");
                    retString.append(getName()).append("(").append(headers[index]).append(")|\n");
                }
                else
                    retString.append(line).append("\n");
                continue;
            }
            for(int j = 0; j < line.split("\\|").length; j++) {
                if(j == index) {
                    numbers.add(Double.parseDouble(line.split("\\|")[j]));
                }
            }
        }

        double number = aggregateOperation(numbers);
        if (number % 1 == 0) {
            retString.append((int) number).append('|');
        } else {
            retString.append(number).append('|');
        }
        System.out.println(retString);
        return retString.toString();
    }

    public static Pattern getPattern() {
        return Pattern.compile("(?i)(?:SUM|AVG|COUNT|MIN|MAX)\\((\\w+)\\)");
    }

}
