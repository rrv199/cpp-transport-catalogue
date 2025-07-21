
#pragma once

#include <deque>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <utility>
#include "geo.h"

namespace TransportSystem {

struct Bus;

struct Stop {
    std::string name_;
    double latitude_;
    double longitude_;


struct Bus {
    std::string name_;
    std::vector<const Stop*> stops_;
    bool is_roundtrip_;
};

struct BusStats {
    size_t stops_count;
    size_t unique_stops_count;
    double route_length;
    double curvature;
};

struct StopPairHash {
    size_t operator()(const std::pair<const Stop*, const Stop*>& p) const {
        auto hash1 = std::hash<const void*>{}(p.first);
        auto hash2 = std::hash<const void*>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

class TransportCatalogue {
public:
    void AddStop(Stop stop);
    void AddBus(Bus bus);
    void AddDistance(std::string_view from, std::string_view to, unsigned distance);
    
    const Stop* GetStop(std::string_view name) const;
    const Bus* GetBus(std::string_view name) const;
    
    BusStats GetBusStats(std::string_view bus_name) const;
    std::set<std::string_view> GetBusesForStop(std::string_view stop_name) const;

private:
    std::deque<Stop> stops_;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, Bus*> busname_to_bus_;
    std::unordered_map<const Stop*, std::set<std::string_view>> stop_to_buses_;
    std::unordered_map<std::pair<const Stop*, const Stop*>, unsigned, StopPairHash> distances_;

    double CalculateGeographicLength(const Bus* bus) const;
    double CalculateRoadLength(const Bus* bus) const;
    size_t CountUniqueStops(const Bus* bus) const;
    unsigned GetDistanceBetweenStops(const Stop* from, const Stop* to) const; 

} // namespace TransportSystem
