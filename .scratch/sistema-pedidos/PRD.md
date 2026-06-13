# Sistema de Pedidos - PRD

Status: ready-for-agent

## Problem Statement

Computer Science students need to develop an academic store management system in C for grading. The project requires creating a robust structure capable of managing entities such as Clients, Products, and Orders. It must support daily, monthly, and annual reports. Furthermore, the system must handle Returns realistically (as an exchange of products, applying fees only from the second return onwards), use pure CSV file data persistence to simulate a relational database, and be perfectly suited to be evaluated via the Windows prompt without the window closing abruptly.

## Solution

The solution is a console application ("CLI") in C, modularized into clear layers (like Interface, Business Rules, and Persistence). The system uses a relational model across multiple `.csv` files for each entity (Client, Product, Order, OrderItem, Return). The system is fail-proof against common console application issues by protecting loops, checking array bounds based on `#define` (such as the daily sales limit), and enforcing string size limits like `MAX_NOME 255`. Windows interactions (`system("pause")` or equivalents) are triggered at screen exit points to allow the evaluator to inspect the results.

## User Stories

1. As a system user, I want to register a Client with a unique ID, name of up to 255 characters, and contact data, so that the system stores who is making the purchases.
2. As a system user, I want to register Products with unique IDs, names (up to 255 chars), and prices, so that they can be added to future orders.
3. As a system user, I want to register an Order associating a Client with multiple Items (Products and quantities), so that I can bill and record the commercial transaction.
4. As a system user, I want to define the quantity of products sold in each OrderItem, so that the system calculates the total value by multiplying it by the quantity and records the unit price frozen at the time of purchase.
5. As a system user, I want to request a Return, fully returning the products of a specific OrderItem, so that the Client performs an exchange (without a monetary refund of the original transaction).
6. As an administrator, I want the system to automatically charge an extra reverse logistics fee of R$ 20.00 starting from the second Return of the same OrderItem by the same Client, so that I cover excessive transport costs.
7. As an evaluating professor, I want to view detailed Daily and Monthly Sales Reports on the screen, so that I verify the data aggregation functionality.
8. As an evaluating professor, I want to view the Annual Report sorted in descending order by each month's revenue, so that I can identify the highest and lowest sales months.
9. As an evaluating professor, I want the terminal window on Windows to pause after executing a routine or presenting a report, so that I have time to read the outputs without the screen closing by itself.
10. As a developer, I want to use configurable constants (`#define`) with large values (e.g., 9999), so that I fulfill the academic requirement of using static arrays without imposing practical volume limits (since the 50 items restriction was removed).
11. As a developer, I want to save all data using a relational CSV format (separate files with IDs referencing each other), so that I ensure persistence between restarts and keep the data normalized without duplicates.

## Implementation Decisions

- **Relational Persistence**: Usage of isolated CSV files (`clientes.csv`, `produtos.csv`, `pedidos.csv`, `itens_pedido.csv`, `devolucoes.csv`) to simulate the relational database model [ADR-0002].
- **Physical String Limits**: Name variables will use static size arrays with `MAX_NOME 255` instead of flexible pointers allocated via `malloc`. This reduces the computational overhead of handling disk allocation for strings of random sizes [ADR-0003].
- **Volume Limits**: The original restriction of 50 items/orders was removed by the professor. To meet the academic obligation of using static arrays, we use `#define` with large values (e.g., 9999) simulating the absence of practical limits [ADR-0001].
- **Windows Compatibility Layer**: Implementation of terminal pauses (like waiting for a keystroke before returning to the main menu or before the `return 0` in main) [ADR-0004].
- **Return Domain Rules**: A return generates an "exchange", not a refund. The logic must search the database (or CSV base) to see if a return linked to that OrderItem already exists. If a prior record is found, the function applies a 20 BRL fee.
- **Sorting Algorithm**: Explicit implementation of a sorting algorithm (e.g., Bubble Sort or Insertion Sort) to organize the months of the Annual Report in descending order of revenue, complying with the technical requirement of the project.

## Testing Decisions

Tests in this academic environment will occur via external behavior verification rather than rigid automated routines, with proper documentation of inputs and outputs.

- Success validation ("Good Test") will consist of performing complete operations via CLI and cross-referencing them with the data filled in the CSV files. We will document the input values used in the tests in a test file and present the respective processed outputs.
- Tested Modules:
  - Base CRUD (Client and Product).
  - Order Management (Generation of `pedido` referencing `itens_pedido`).
  - Return Flow (testing the occurrence of the first clean Return and the second with the application of the 20 BRL fee).
  - Report Generation (Validation of profit calculations in reports and correct sorting in the Annual Report).
- Boundary Tactics: Strings larger than 255 chars will be inserted to ensure the system protects these memory boundaries and informs the user through the Terminal layer.

## Out of Scope

- Graphical User Interface (GUI) and Web (Focused 100% on C CLI Terminal).
- Native SQL Databases (SQLite, MySQL, Postgres). Text files (CSV) must be used under the same IO limiting constraints.
- Concurrent parallel management and Thread Safeness. Since it's a single-player console app for an academic project, there will be no mitigation of concurrency race conditions with the same CSV file.
- Real return shipping logistics (package tracking, delivery status - irrelevant for this scope).

## Further Notes

The application uses `CONTEXT.md` in the root for the Glossary and `docs/adr/` for Architectural Decision Records. Always consult such directories before expanding classes or structs.
