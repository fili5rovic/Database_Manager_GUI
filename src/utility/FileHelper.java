package utility;

import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.*;
import java.util.*;

public class FileHelper {
    public static File selectDatabaseFile(String path, Stage currentStage) {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Select Database");
        FileChooser.ExtensionFilter dbFilter = new FileChooser.ExtensionFilter("Database files (*.wyl, *.sql)", "*.wyl", "*.sql");
        fileChooser.getExtensionFilters().add(dbFilter);
        fileChooser.setInitialDirectory(new File(path));
        return fileChooser.showOpenDialog(currentStage);
    }

    public static String readFromFile(String path) {
        StringBuilder sb = new StringBuilder();
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;
            while ((line = br.readLine()) != null) {
                sb.append(line).append('\n');
            }
            if (sb.length() > 0)
                sb.deleteCharAt(sb.length() - 1);
        } catch (IOException e) {
            System.out.println("Couldn't read file " + path);
        }
        return sb.toString();
    }

    public static void writeToFile(String path, String content) {
        try (java.io.FileWriter fw = new java.io.FileWriter(path)) {
            fw.write(content);
        } catch (IOException e) {
            System.out.println("Couldn't write to file " + path);
        }
    }

    public static void limitFileLines(int limit,int offset, String path) {
        List<String> lines = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;
            while ((line = br.readLine()) != null) {
                lines.add(line);
            }
        } catch (IOException e) {
            System.out.println("Couldn't read file " + path);
            return;
        }

        offset = Math.min(offset, lines.size());
        try (java.io.FileWriter fw = new java.io.FileWriter(path)) {
            System.out.println("LIMIT: " + limit + " OFFSET: " + offset);
            fw.write(lines.get(0) + System.lineSeparator()); // table name
            fw.write(lines.get(1) + System.lineSeparator()); // table columns
            for (int i = 2; i < lines.size(); i++) {
                if (limit == 0) {
                    break;
                }
                if(offset > 0) {
                    offset--;
                } else {
                    fw.write(lines.get(i) + System.lineSeparator());
                    limit--;
                }
            }
        } catch (IOException e) {
            System.out.println("Couldn't write to file " + path);
        }
    }

    public static String readFromFileNoDuplicates(String path) {
        StringBuilder sb = new StringBuilder();
        List<String> lines = new ArrayList<>();
        boolean isFirstLine = true;
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;
            while ((line = br.readLine()) != null) {
                if (!lines.contains(line)) {
                    if(isFirstLine) {
                        isFirstLine = false;
                    } else {
                        lines.add(line);
                    }
                    sb.append(line).append('\n');
                }
            }
            if (sb.length() > 0)
                sb.deleteCharAt(sb.length() - 1);
        } catch (IOException e) {
            System.out.println("Couldn't read file " + path);
        }
        return sb.toString();
    }


    public static String convertSQLToWYL(String sql) {
        StringBuilder wyl = new StringBuilder();
        String[] statements = sql.split(";");
        String correctStatement;
        for(String statement : statements) {
            statement = statement.trim();
            if(statement.startsWith("CREATE TABLE IF NOT EXISTS")) {
                wyl.append("START_TABLE ");
                correctStatement = statement.replace("CREATE TABLE IF NOT EXISTS","");
                correctStatement = correctStatement.replaceAll("VARCHAR\\(50\\) NOT NULL","");
                String[] splitStatement = correctStatement.split("\\(");
                String tableName = splitStatement[0].trim();
                splitStatement[1] = splitStatement[1].trim();
                splitStatement[1] = splitStatement[1].replace(")","");
                String[] columns = splitStatement[1].split("\\s*,\\s*");
                columns[columns.length-1] = columns[columns.length-1].replaceAll("\\t+","");
                wyl.append(tableName);
                for(String col : columns) {
                    wyl.append("\n   ").append(col);
                }
                wyl.append("END_TABLE").append('\n');
            }
        }
        wyl.append("START_DATA\n");
        for(String statement : statements) {
            statement = statement.trim();
            if(statement.startsWith("INSERT INTO")) {
                statement = statement.replace("INSERT INTO ","");
                String[] splitString = statement.split("VALUES");
                String tableName = splitString[0].trim();
                String values = splitString[1].trim();
                values = values.replace('(','[');
                values = values.replace(')',']');
                wyl.append("\t[").append(tableName).append("] ADD ").append(values).append('\n');
            }
        }
        wyl.append("END_DATA");
        System.out.println(wyl);

        return wyl.toString();
    }


//    public static void main(String[] args) {
//        System.out.println(convertSQLToWYL("CREATE TABLE IF NOT EXISTS \"Professor\" (\n" +
//                "   \"ProfessorID\"\tVARCHAR(50) NOT NULL,\n" +
//                "          \"Name\"\tVARCHAR(50) NOT NULL,\n" +
//                "       \"Surname\"\tVARCHAR(50) NOT NULL,\n" +
//                "           \"Age\"\tVARCHAR(50) NOT NULL\n" +
//                ");\n" +
//                "CREATE TABLE IF NOT EXISTS \"Student\" (\n" +
//                "        \"ID\"\tVARCHAR(50) NOT NULL,\n" +
//                "      \"Name\"\tVARCHAR(50) NOT NULL,\n" +
//                "   \"Surname\"\tVARCHAR(50) NOT NULL\n" +
//                ");\n" +
//                "CREATE TABLE IF NOT EXISTS \"Grades\" (\n" +
//                "     \"Index\"\tVARCHAR(50) NOT NULL,\n" +
//                "   \"SubName\"\tVARCHAR(50) NOT NULL,\n" +
//                "     \"Grade\"\tVARCHAR(50) NOT NULL\n" +
//                ");\n" +
//                "INSERT INTO \"Professor\" VALUES ('100','Tatjana','Lutovac','48');\n" +
//                "INSERT INTO \"Student\" VALUES ('192','Filip','Petrovic');\n" +
//                "INSERT INTO \"Student\" VALUES ('589','Jovan','Mosurovic');\n" +
//                "INSERT INTO \"Student\" VALUES ('175','Lazar','Stankovic');\n" +
//                "INSERT INTO \"Student\" VALUES ('445','Ilija','Jankovic');\n" +
//                "INSERT INTO \"Grades\" VALUES ('192','Fizika','7');\n" +
//                "INSERT INTO \"Grades\" VALUES ('589','Fizika','7');\n" +
//                "INSERT INTO \"Grades\" VALUES ('175','Fizika','7');\n"));
//    }

}
