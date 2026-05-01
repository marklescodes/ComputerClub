# Computer Club Workflow Engine 🖥️

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Cross--platform-lightgrey.svg)
![Build](https://img.shields.io/badge/Build-CMake-orange.svg)

A high-performance C++ console engine that simulates the operational workflow of a computer club. The system processes an event-driven stream (client arrivals, seat assignments, queuing) and calculates precise financial statistics based on custom business rules. 

*Developed as part of the technical assessment for YADRO.*

## 🚀 Key Features
- **Event-Driven Architecture**: Sequential processing of complex client scenarios with millisecond-accurate time accounting.
- **Strict Business Logic**: Implements sophisticated rules for:
  - Table occupancy and queue management.
  - Arriving before/after business hours.
  - Automated client expulsion at closing time.
- **Financial Analytics**: Precise revenue calculation with hourly rounding (ceiling approach) and table utilization tracking.
- **Robust Input Parsing**: Validates structured text input and handles logical edge cases with custom error reporting.

## 🛠 Prerequisites

- **Compiler**: GCC 9+, Clang 10+, or MSVC (C++17 support required).
- **Build System**: CMake 3.10 or higher.
- **Standard Library**: Uses STL containers (std::map, std::deque, std::vector).

## ⚙️ Installation & Build
- Clone the repository
```bash
git clone https://github.com/marklescodes/ComputerClub
cd computer-club-simulation
```

- Build the project
``` bash
mkdir build && cd build
cmake ..
make
```

## 📈 Usage

The application operates via the command line and expects a path to a configuration file:

``` bash
./task ../test_file.txt
```

Input File Format:

``` text

3                     # Number of tables
09:00 19:00           # Operating hours
10                    # Hourly rate
08:48 1 client1       # Event: Time, ID, Payload
...
```
