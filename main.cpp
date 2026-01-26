#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <deque>
#include <map>


struct Time {

    int h = 0;
    int m = 0;

    static Time parse(const std::string& s) {
        if (s.size() != 5; s[2] != ':') throw std::runtime_error ("Format");
        int hours = stoi(s.substr(0, 2));
        int minutes = stoi(s.substr(3, 2));
        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 60) throw std::runtime_error ("Format");
        return {hours, minutes};
    }

    int toMinutes() const {
        return h * 60 + m;
    }

    bool operator<(const Time& other) const {
        return toMinutes() < other.toMinutes();
    }

    bool operator>(const Time& other) const {
        return toMinutes() > other.toMinutes();
    }

    bool operator<=(const Time& other) const {
        return toMinutes() <= other.toMinutes();
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << h << ":" << std::setfill('0') << std::setw(2) << m;
        return oss.str();
    }
};

struct Event {
    Time time;
    int id;
    std::string clientName;
    int tableNumber = 0;
    std::string originalLine;
};

struct Table {
    int id;
    int revenue = 0;
    int timeOccupied = 0;
    bool isBusy = false;
};

struct Client {
    std::string name;
    bool isInside = false;
};

class ComputerClub {
private:
    int numTables;
    int pricePerHour;
    Time startTime;
    Time endTime;

    std::vector<Table> tables;
    std::map<std::string, Client> clients;

public:
    ComputerClub(int n, int price, Time start, Time end) :
        numTables(n), pricePerHour(price), startTime(start), endTime(end) {
            for (int i = 1; i <= n; ++i) tables.push_back({i});
    }

    void processEvents(const std::vector<Event>& events) {
        std::cout << startTime.toString() << std::endl;

        for (const auto& event : events) {
            std::cout << event.time.toString() << " " << event.id << " " << event.clientName;
            if (event.id == 2 || event.id == 12) std::cout << " " << event.tableNumber;
            std::cout << std::endl;

            switch (event.id) {
                case 1:
                    handleClientArrived(event);
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
            }
        }

        std::cout << endTime.toString() << std::endl;
    }

private:
    void handleClientArrived(const Event& e) {
        if (e.time < startTime) {
            printError(e.time, "NotOpenYet");
            return;
        }

        if (clients.count(e.clientName) && clients[e.clientName].isInside) {
            printError(e.time, "YouShallNotPass");
            return;
        }

        if (clients.find(e.clientName) == clients.end()) clients[e.clientName] = {e.clientName, false};

        clients[e.clientName].isInside = true;
    }

    void printError(const Time& t, const std::string& msg) {
        std::cout << t.toString() << " 13 " << msg << std::endl;
    }
};


int NUM_TABLES;
Time START_TIME;
Time END_TIME;
int PRICE_PER_HOUR;
std::vector<Event> EVENTS;

bool isValidName(std::string name) {
    for (char c : name) {
        if (!isalnum(c) && c != '_' && c != '-') return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Укажите файл" << std::endl;
        return 0;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << "Файл не найден" << std::endl;
        return 0;
    }

    std::string line;

    if (!getline(file, line)) return 0;
    try {
        NUM_TABLES = std::stoi(line);
        if (NUM_TABLES <= 0) throw std::runtime_error("Tables <= 0");
    } catch (...) {
        std::cout << line << std::endl;
        return 0;
    }

    if (!getline(file, line)) return 0;
    try {
        std::stringstream ss(line);
        std::string startS, endS;
        ss >> startS >> endS;
        START_TIME = Time::parse(startS);
        END_TIME = Time::parse(endS);
    } catch (...) {
        std::cout << line << std::endl;
        return 0;
    }

    if (!std::getline(file, line)) return 0;
    try {
        PRICE_PER_HOUR = stoi(line);
        if (PRICE_PER_HOUR < 0) throw std::runtime_error("Price < 0");
    } catch (...) {
        std::cout << line << std::endl;
        return 0;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;

        try {
            std::stringstream ss(line);
            std::string timeS, name;
            int id;

            ss >> timeS >> id >> name;

            if (ss.fail()) throw std::runtime_error("Parse error");
            if (!isValidName(name)) throw std::runtime_error("Invalid name");

            Event e;
            e.time = Time::parse(timeS);
            e.id = id;
            e.clientName = name;
            e.originalLine = line;

            if (id == 2) {
                int table;
                ss >> table;
                if (ss.fail()) throw std::runtime_error("No table num");
                if (table < 1 || table > NUM_TABLES) throw std::runtime_error("Bad table num");
                e.tableNumber = table;
            } else if (id < 1 || id > 4) {
                throw std::runtime_error("Unknown ID");
            }

            EVENTS.push_back(e);

        } catch (...) {
            std::cout << line << std::endl;
            return 0;
        }
    }

    ComputerClub club(NUM_TABLES, PRICE_PER_HOUR, START_TIME, END_TIME);
    club.processEvents(EVENTS);
    
    return 0;
}