#pragma once
#include <deque>
#include <string>
#include <vector>

#include <iomanip>
#include <iostream>

#include <unordered_set>
#include <unordered_map>

#include "geo.h"
namespace TransportSystem {
struct Bus;
    
struct Stop {    
    std::string name_;
    double latitude_;
    double longitude_;
    
    std::vector<Bus*> buses_;
};

struct Bus { 
    std::string name_;
    std::vector<Stop*> stops_;
};

typedef std::unordered_map<std::string_view , Stop*> StopMap;
typedef std::unordered_map<std::string_view , Bus*> BusMap;


class TransportCatalogue {
public:   
    void add_bus(Bus&& bus);
    void add_stop(Stop&& stop);
    
    Bus* get_bus(std::string_view _bus_name);
    Stop* get_stop(std::string_view _stop_name);

    std::unordered_set<const Bus*> stop_get_uniq_buses(Stop* stop);    
    std::unordered_set<const Stop*> get_uniq_stops(Bus* bus);
    double get_length(Bus* bus);
private:

    std::deque<Stop> stops_;
    StopMap stopname_to_stop;
    
    std::deque<Bus> buses_;
    BusMap busname_to_bus;
};
}
