#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

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

    std::cout << "Start: " << START_TIME.toString() << std::endl;
    std::cout << "End: " << END_TIME.toString() << std::endl;
    std::cout << "Price: " << PRICE_PER_HOUR << std::endl;
    std::cout << "Tables: " << NUM_TABLES << std::endl;
    std::cout << "--- Events ---" << std::endl;
    for (const auto& ev: EVENTS) {
        std::cout << ev.time.toString() << " " << ev.id << " " << ev.clientName;
        if (ev.id == 2) std::cout << " table:" << ev.tableNumber;
        std::cout << std::endl;
    }

    return 0;
}