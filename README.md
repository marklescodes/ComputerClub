# Computer Club Simulation 

A C++ console application that simulates the daily workflow of a computer club. The program processes a stream of events (client arrival, waiting in queue, sitting at a table, leaving) and calculates the daily revenue based on specific business rules.

This solution was developed as part of a technical assessment (YADRO).

## Features

- **Tech Stack:** C++17, STL.
- **Build System:** CMake (Cross-platform).
- **Architecture:** OOP approach. The club logic is encapsulated within the `ComputerClub` class.
- **Algorithms:** Efficient usage of `std::deque` for the waiting queue and `std::map` for quick client lookups.
- **Robustness:** Strict input validation and handling of logical errors (e.g., trying to sit at an occupied table, arriving before opening hours).

## Project Structure

- `main.cpp` — Main source code.
- `CMakeLists.txt` — CMake configuration file.
- `test_file.txt` — Sample input data for testing.

## Build and Run

To build this project, you will need:
- A C++ compiler supporting C++17 (GCC, Clang).
- **CMake** (version 3.10 or higher).
- **Make**.

### Linux and macOS

1. **Clone the repository** (or navigate to the project directory):
   ```bash
   cd path/to/project

2. **Create a build directory**:

    ```bash
    mkdir build
    cd build

3. **Generate build files and compile**:

     ```bash
  
     cmake ..
     make

  Upon successful compilation, an executable named task will appear in the build directory.

4. **Run the application**:
   
    The program requires the path to the input file as an argument.
  
    ```bash
  
    ./task ../test_file.txt


### Windows (MinGW / Visual Studio)

     mkdir build
     cd build
     cmake ..
     cmake --build .
     task.exe ..\test_file.txt

## Input format

The program expects a text file with the following structure:

    <Number of tables>
    <Open time> <Close time>
    <Hourly rate>
    <Event time> <ID> <Event body>
    ...

Example (test_file.txt):

    3
    09:00 19:00
    10
    08:48 1 client1
    09:41 1 client1
    09:48 1 client2
    ...

## Output Example

**Console Output**:
The program prints the timeline of events, including errors and generated events (like clients leaving or sitting down from the queue), followed by the final statistics for each table.

    09:00
    08:48 13 NotOpenYet
    09:41 1 client1
    09:48 1 client2
    09:52 13 ICanWaitNoLonger!
    09:54 2 client1 1
    ...
    19:00 11 client3
    19:00
    1 70 05:58
    2 30 02:18
    3 90 08:01
