
#include "stat_reader.h"
#include <iomanip>

namespace TransportSystem::Stat {

using namespace std;

void PrintBusInfo(const TransportCatalogue& catalogue, string_view bus_name, ostream& output) {
    auto stats = catalogue.GetBusStats(bus_name);
    if (stats.stops_count == 0) {
        output << "Bus " << bus_name << ": not found" << endl;
    } else {
        output << "Bus " << bus_name << ": " 
               << stats.stops_count << " stops on route, "
               << stats.unique_stops_count << " unique stops, "
               << fixed << setprecision(6) << stats.route_length << " route length, "
               << stats.curvature << " curvature" << endl;
    }
}

void PrintStopInfo(const TransportCatalogue& catalogue, string_view stop_name, ostream& output) {
    auto buses = catalogue.GetBusesForStop(stop_name);
    if (catalogue.GetStop(stop_name) == nullptr) {
        output << "Stop " << stop_name << ": not found" << endl;
    } else if (buses.empty()) {
        output << "Stop " << stop_name << ": no buses" << endl;
    } else {
        output << "Stop " << stop_name << ": buses";
        for (string_view bus : buses) {
            output << " " << bus;
        }
        output << endl;
    }
}

void ProcessQueries(const TransportCatalogue& catalogue, std::istream& input, std::ostream& output) {
    string line;
    getline(input, line);
    int query_count = stoi(line);
    
    for (int i = 0; i < query_count; ++i) {
        getline(input, line);
        if (line.empty()) continue;
        
        if (line.substr(0, 4) == "Bus ") {
            PrintBusInfo(catalogue, line.substr(4), output);
        } else if (line.substr(0, 5) == "Stop ") {
            PrintStopInfo(catalogue, line.substr(5), output);
        }
    }
}

} // namespace TransportSystem::Stat
