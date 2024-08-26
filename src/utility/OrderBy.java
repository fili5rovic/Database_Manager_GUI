package utility;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;
import java.util.stream.Collectors;

public class OrderBy {

    private static List<String[]> readData(String filename) {
        List<String[]> data = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            int i = 0;
            while ((line = br.readLine()) != null) {
                i++;
                if (i <= 3) {
                    i++;
                    continue;
                }
                data.add(line.split("\\|"));
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return data;
    }

    private static List<String> readLine(String filename, int num){
        List<String> headers = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            int i = 0;
            while ((line = br.readLine()) != null) {
                if (i == num) {
                    headers = Arrays.asList(line.split("\\|"));
                    break;
                }
                i++;
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return headers;
    }

    private static List<String[]> sortData(String path, List<String[]> data, List<String> orderBy) {
        Map<String, Integer> columnMap = new HashMap<>();

        for(int i = 0; i < readLine(path,1).size(); i++) {
            columnMap.put(readLine(path,1).get(i).toLowerCase(), i);
        }

        Comparator<String[]> comparator = null;

        for (String order : orderBy) {
            String[] parts = order.split(" ");
            String column = parts[0].toLowerCase();
            boolean ascending = parts[1].equalsIgnoreCase("ASC");

            Comparator<String[]> columnComparator = Comparator.comparing(row -> row[columnMap.get(column)]);
            if (!ascending) {
                columnComparator = columnComparator.reversed();
            }

            comparator = (comparator == null) ? columnComparator : comparator.thenComparing(columnComparator);
        }

        return data.stream().sorted(comparator).collect(Collectors.toList());
    }

    public static void writeOrderedToFile(String path, List<String> orderBy) {
        List<String[]> data = readData(path);

        // Example: Order by ID ASC, Name DESC
        List<String[]> sortedData = sortData(path, data, orderBy);
        sortedData.add(0, readLine(path,0).toArray(new String[0])); // Table name
        sortedData.add(1, readLine(path,1).toArray(new String[0])); // Table columns

        try (java.io.FileWriter fw = new java.io.FileWriter(path)) {
            boolean firstLine = true;
            for (String[] row : sortedData) {
                fw.write(String.join("|", row) + (firstLine ? "\n":"|\n"));
                firstLine = false;
            }
        } catch (IOException e) {
            System.out.println("Couldn't write to file " + path);
        }


    }

    public static void main(String[] args) throws IOException {
        writeOrderedToFile("output.txt", new ArrayList<>(Arrays.asList("ID DESC", "Name DESC")));
    }
}
