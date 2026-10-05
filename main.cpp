#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

using std::cin;
using std::cout;
using std::string;
using std::vector;

struct ip {
    int o1;
    int o2;
    int o3;
    int o4;
};

struct event {
    struct std::tm ts;
    struct ip ip_o;
    string port_o;
    string domain_o;
    struct ip ip_d;
    string port_d;
    string domain_d;
};

std::ostream& operator<<(std::ostream &os, const ip &i) {
    if (i.o1 == 0) {
        os << "-";
    } else {
        os << i.o1 << "." << i.o2 << "." << i.o3 << "." << i.o4;
    }
    return os;
}

std::ostream& operator<<(std::ostream &os, const event &e) {
    char dateOutput[20];
    strftime(dateOutput, 20, "%d-%m-%Y,%T", &e.ts);
    os << dateOutput << "," << e.ip_o << "," << e.port_o << ","
       << e.domain_o << "," << e.ip_d << "," << e.port_d << ","
       << e.domain_d;
    return os;
}

std::time_t toTime(std::tm date) {
    return std::mktime(&date);
}

bool operator<(const event &e1, const event &e2) {
    return toTime(e1.ts) < toTime(e2.ts);
}

ip parseIp(const string &text) {
    ip result{0, 0, 0, 0};
    if (text == "-") {
        return result;
    }
    char dot = ' ';
    std::istringstream ss(text);
    ss >> result.o1 >> dot >> result.o2 >> dot >> result.o3 >> dot >> result.o4;
    return result;
}

event parseEvent(const string &line) {
    std::istringstream ss(line);
    string date = "";
    string time = "";
    string ipOrigin = "";
    string ipDestination = "";
    event e{};

    std::getline(ss, date, ',');
    std::getline(ss, time, ',');
    std::getline(ss, ipOrigin, ',');
    std::getline(ss, e.port_o, ',');
    std::getline(ss, e.domain_o, ',');
    std::getline(ss, ipDestination, ',');
    std::getline(ss, e.port_d, ',');
    std::getline(ss, e.domain_d, ',');

    std::istringstream dateTime(date + " " + time);
    dateTime >> std::get_time(&e.ts, "%d-%m-%Y %H:%M:%S");
    e.ts.tm_isdst = -1;

    e.ip_o = parseIp(ipOrigin);
    e.ip_d = parseIp(ipDestination);
    return e;
}

vector<event> readLog(const string &fileName) {
    vector<event> events{};
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << fileName << "\n";
        return events;
    }
    string line = "";
    while (std::getline(file, line)) {
        if (!line.empty()) {
            events.push_back(parseEvent(line));
        }
    }
    return events;
}

bool writeLog(const string &fileName, const vector<event> &events) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "No se pudo crear el archivo: " << fileName << "\n";
        return false;
    }
    for (const event &e : events) {
        file << e << "\n";
    }
    return true;
}

std::tm askDate(const string &message) {
    std::tm date{};
    string input = "";
    bool valid = false;
    while (!valid) {
        cout << message;
        cin >> input;
        date = std::tm{};
        std::istringstream ss(input);
        ss >> std::get_time(&date, "%d-%m-%Y");
        valid = !ss.fail();
        if (!valid) {
            cout << "Formato invalido, usa dd-mm-aaaa\n";
        }
    }
    date.tm_isdst = -1;
    return date;
}

void printFrom(const vector<event> &events, const std::tm &date) {
    std::time_t dateTime = toTime(date);

    auto first = std::find_if(events.begin(), events.end(),
        [dateTime](const event &e) { return toTime(e.ts) >= dateTime; });

    for (auto it = first; it != events.end(); ++it) {
        cout << *it << "\n";
    }
    cout << "Eventos encontrados: " << (events.end() - first) << "\n";
}

int main() {
    const string INPUT_FILE = "equipo5.csv";
    const string OUTPUT_FILE = "equipo5_ordenado.csv";

    vector<event> events = readLog(INPUT_FILE);
    if (events.empty()) {
        return 1;
    }
    cout << "Eventos leidos: " << events.size() << "\n";

    std::sort(events.begin(), events.end());

    if (writeLog(OUTPUT_FILE, events)) {
        cout << "Bitacora ordenada guardada en " << OUTPUT_FILE << "\n";
    }

    std::tm date = askDate("Fecha (dd-mm-aaaa): ");
    printFrom(events, date);

    return 0;
}
