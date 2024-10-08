# Database Manager GUI
This project is a Java-based application that provides a user interface for managing and interacting with a database. It includes features such as running SQL queries, displaying query results, and managing database tables. The application uses JavaFX for the user interface.

## Features
- **Run SQL Queries**: Execute SQL queries and display the results in a tabular format.
- **Database Management**: Create, drop, and manage database tables.
- **Theming**: Switch between light and dark themes.
- **Code Highlighting**: Syntax highlighting for SQL code.
- **Aggregates**: Support for aggregate functions like SUM, AVG, COUNT, MIN, and MAX.
- **Pagination**: Limit and offset support for query results.
- **Order By**: Sort query results by specified columns.

## Project Structure
- `src/controller`: Contains the main controller class `DashboardController.java` which handles the user interface and functionality.
- `src/utility`: Contains utility classes for file operations, theme selection, and other helper functions.
- `src/db`: Contains the `DBManager.java` class for handling database operations.
- `src/light_theme.css` and `src/dark_theme.css`: CSS files for the light and dark themes.

## Getting Started

### Prerequisites
- Java Development Kit (JDK) 8 or higher
- IntelliJ IDEA or any other Java IDE
- JavaFX SDK

### Installation
1. Clone the repository:
    ```sh
    git clone https://github.com/fili5rovic/Database_Manager_GUI.git
    ```
2. Open the project in IntelliJ IDEA.
3. Ensure that the JavaFX SDK is properly configured in your IDE.


## Usage
- **Run SQL Queries**: Enter your SQL queries in the provided text area and click the "Run" button.
- **Switch Themes**: Use the menu options to switch between light and dark themes.
- **Manage Tables**: Use the tree view to select and manage database tables.

## Contributing
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes and commit them (`git commit -m 'Add some feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a pull request.

