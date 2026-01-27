#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <deque>
#include <map>
#include <algorithm>

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
    std::string clientName = "";
    Time startTime;
};

struct Client {
    std::string name;
    bool isInside = false;

    int tableId = -1;
};

class ComputerClub {
private:
    int numTables;
    int pricePerHour;
    Time startTime;
    Time endTime;

    std::vector<Table> tables;
    std::map<std::string, Client> clients;
    std::deque<std::string> waitQueue;

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

        closeClub();
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

    std::string formatDuration(int totalMinutes) {
        int h = totalMinutes / 60;
        int m = totalMinutes % 60;
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << h << ":" << std::setfill('0') << std::setw(2) << m;
        return oss.str();
    }

    void printError(const Time& t, const std::string& msg) {
        std::cout << t.toString() << " 13 " << msg << std::endl;
    }

    void sitDownTable(int tableIndex, Time currentTime) {
        Table& t = tables[tableIndex];
        
        if (!t.isBusy) return;
        int duration = currentTime.toMinutes() - t.startTime.toMinutes();
        t.timeOccupied += duration;

        int hours = (duration + 59) / 60;
        t.revenue += hours * pricePerHour;

        t.isBusy = false;
        t.clientName = "";
    }

    void handleClientSit(const Event& e) {
        if (clients.find(e.clientName) == clients.end() || !clients[e.clientName].isInside) {
            printError(e.time, "ClientUnknown");
            return;
        }

        if (tables[e.tableNumber - 1].isBusy) {
            printError(e.time, "PlaceIsBusy");
            return;
        }

        Client& client = clients[e.clientName];
        if (client.tableId != -1) {
            sitDownTable(client.tableId - 1, e.time);
        }

        Table& t = tables[e.tableNumber - 1];
        t.isBusy = true;
        t.clientName = e.clientName;
        t.startTime = e.time;

        client.tableId = e.tableNumber;
    }

    void handleClientWait(const Event& e) {
        for (const auto& t : tables) {
            if (!t.isBusy) {
                printError(e.time, "ICanWaitNoLonger");
                return;
            }
        }

        if (waitQueue.size() >= (size_t)numTables) {
            std::cout << e.time.toString() << " 11 " << e.clientName << std::endl;
            clients[e.clientName].isInside = false;
        } else {
            waitQueue.push_back(e.clientName);
        }
    }

    void handleClientLeave(const Event& e) {
        if (clients.find(e.clientName) == clients.end() || !clients[e.clientName].isInside) {
            printError(e.time, "ClientUnknown");
            return;
        }

        Client& client = clients[e.clientName];
        int freedTableId = client.tableId;

        if (freedTableId != -1) {
            sitDownTable(freedTableId - 1, e.time);
            client.tableId = -1;
        }

        client.isInside = false;

        if (freedTableId != -1 && !waitQueue.empty()) {
            std::string nextClientName = waitQueue.front();
            waitQueue.pop_front();

            std::cout << e.time.toString() << " 12 " << nextClientName << " " << freedTableId << std::endl;

            Table& t = tables[freedTableId - 1];
            t.isBusy = true;
            t.clientName = nextClientName;
            t.startTime = e.time;

            clients[nextClientName].tableId = freedTableId;
        }
    }

    void closeClub() {
        std::vector<std::string> remainingClients;
        for (auto const& [name, client] : clients) {
            if (client.isInside) {
                remainingClients.push_back(name);
            }
        }

        std::sort(remainingClients.begin(), remainingClients.end());

        for (const std::string& name : remainingClients) {
            Client& c = clients[name];

            std::cout << endTime.toString() << " 11 " << name << std::endl;

            if (c.tableId != -1) {
                sitDownTable(c.tableId - 1, endTime);
                c.tableId = -1;
            }

            c.isInside = false;
        }

        std::cout << endTime.toString() << std:: endl;

        for (const auto& t : tables) {
            std::cout << t.id << " " << t.revenue << " " << formatDuration(t.timeOccupied) << std::endl;
        }
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