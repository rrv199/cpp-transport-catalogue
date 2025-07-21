#include "input_reader.h"
#include "transport_catalogue.h"
#include <sstream>
#include <algorithm>

namespace TransportSystem::Input {

using namespace std;

Stop ParseStop(string_view line) {
    Stop stop;
    size_t colon_pos = line.find(':');
    stop.name_ = line.substr(5, colon_pos - 5); // Remove "Stop "
    
    line = line.substr(colon_pos + 2); // Skip ": "
    size_t comma_pos = line.find(',');
    stop.latitude_ = stod(string(line.substr(0, comma_pos)));
    stop.longitude_ = stod(string(line.substr(comma_pos + 2)));
    
    return stop;
}

void ParseDistances(TransportCatalogue& catalogue, string_view line, const string& stop_name) {
    size_t colon_pos = line.find(':');
    string_view dist_info = line.substr(colon_pos + 1);
    size_t dist_pos = dist_info.find("m to ");
    
    while (dist_pos != string_view::npos) {
        size_t num_start = dist_info.substr(0, dist_pos).rfind(' ') + 1;
        unsigned distance = stoul(string(dist_info.substr(num_start, dist_pos - num_start)));
        
        size_t to_pos = dist_pos + 5;
        size_t next_comma = dist_info.find(',', to_pos);
        string_view to_stop = dist_info.substr(to_pos, next_comma == string_view::npos ? 
                                        dist_info.size() - to_pos : next_comma - to_pos);
        
        // Trim whitespace
        while (!to_stop.empty() && isspace(to_stop.front())) to_stop.remove_prefix(1);
        while (!to_stop.empty() && isspace(to_stop.back())) to_stop.remove_suffix(1);
        
        catalogue.AddDistance(stop_name, to_stop, distance);
        
        if (next_comma != string_view::npos) {
            dist_info = dist_info.substr(next_comma + 2);
            dist_pos = dist_info.find("m to ");
        } else {
            break;
        }
    }
}

Bus ParseBus(TransportCatalogue& catalogue, string_view line) {
    Bus bus;
    size_t colon_pos = line.find(':');
    bus.name_ = line.substr(4, colon_pos - 4); // Remove "Bus "
    
    line = line.substr(colon_pos + 2); // Skip ": "
    bool is_roundtrip = line.find('>') != string_view::npos;
    bus.is_roundtrip_ = is_roundtrip;
    
    string_view delimiter = is_roundtrip ? " > " : " - ";
    size_t delimiter_pos;
    
    do {
        delimiter_pos = line.find(delimiter);
        string_view stop_name = line.substr(0, delimiter_pos);
        
        // Trim whitespace
        while (!stop_name.empty() && isspace(stop_name.front())) stop_name.remove_prefix(1);
        while (!stop_name.empty() && isspace(stop_name.back())) stop_name.remove_suffix(1);
        
        if (const Stop* stop = catalogue.GetStop(stop_name)) {
            bus.stops_.push_back(stop);
        }
        
        if (delimiter_pos != string_view::npos) {
            line = line.substr(delimiter_pos + delimiter.size());
        }
    } while (delimiter_pos != string_view::npos);
    
    // For non-roundtrip buses, add stops in reverse order (except last)
    if (!is_roundtrip && bus.stops_.size() > 1) {
        for (size_t i = bus.stops_.size() - 1; i-- > 0; ) {
            bus.stops_.push_back(bus.stops_[i]);
        }
    }
    
    return bus;
}

void ParseAndLoadData(TransportCatalogue& catalogue, istream& input) {
    string line;
    getline(input, line);
    int query_count = stoi(line);
    
    vector<string> bus_lines;
    vector<string> stop_lines;
    
    // First pass - collect all stops and buses
    for (int i = 0; i < query_count; ++i) {
        getline(input, line);
        if (line.empty()) continue;
        
        if (line.substr(0, 5) == "Stop ") {
            stop_lines.push_back(line);
        } else if (line.substr(0, 4) == "Bus ") {
            bus_lines.push_back(line);
        }
    }
    
    // Second pass - add all stops
    for (string& stop_line : stop_lines) {
        catalogue.AddStop(ParseStop(stop_line));
    }
    
    // Third pass - add all distances
    for (string& stop_line : stop_lines) {
        size_t colon_pos = stop_line.find(':');
        string stop_name = string(stop_line.substr(5, colon_pos - 5));
        ParseDistances(catalogue, stop_line, stop_name);
    }
    
    // Fourth pass - add all buses
    for (string& bus_line : bus_lines) {
        catalogue.AddBus(ParseBus(catalogue, bus_line));
    }
}

} // namespace TransportSystem::Input
