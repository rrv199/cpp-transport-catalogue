
#include "transport_catalogue.h"

namespace TransportSystem {

void TransportCatalogue::AddStop(Stop stop) {
    stops_.push_back(std::move(stop));
    Stop* stop_ptr = &stops_.back();
    stopname_to_stop_[stop_ptr->name_] = stop_ptr;
}

void TransportCatalogue::AddBus(Bus bus) {
    buses_.push_back(std::move(bus));
    Bus* bus_ptr = &buses_.back();
    busname_to_bus_[bus_ptr->name_] = bus_ptr;
    
    for (const Stop* stop : bus_ptr->stops_) {
        stop_to_buses_[stop].insert(bus_ptr->name_);
    }
}

void TransportCatalogue::AddDistance(std::string_view from, std::string_view to, unsigned distance) {
    const Stop* from_stop = GetStop(from);
    const Stop* to_stop = GetStop(to);
    
    if (from_stop && to_stop) {
        distances_[{from_stop, to_stop}] = distance;
    }
}

const Stop* TransportCatalogue::GetStop(std::string_view name) const {
    auto it = stopname_to_stop_.find(name);
    return it != stopname_to_stop_.end() ? it->second : nullptr;
}

const Bus* TransportCatalogue::GetBus(std::string_view name) const {
    auto it = busname_to_bus_.find(name);
    return it != busname_to_bus_.end() ? it->second : nullptr;
}

unsigned TransportCatalogue::GetDistanceBetweenStops(const Stop* from, const Stop* to) const {
    if (!from || !to) return 0;
    
    // Check direct distance
    auto it = distances_.find({from, to});
    if (it != distances_.end()) {
        return it->second;
    }
    
    // Check reverse distance
    it = distances_.find({to, from});
    if (it != distances_.end()) {
        return it->second;
    }
    
    // Return geographic distance as fallback
    return static_cast<unsigned>(ComputeDistance(
        {from->latitude_, from->longitude_},
        {to->latitude_, to->longitude_}
    ));
}

double TransportCatalogue::CalculateGeographicLength(const Bus* bus) const {
    if (!bus || bus->stops_.size() < 2) return 0.0;
    
    double length = 0.0;
    for (size_t i = 1; i < bus->stops_.size(); ++i) {
        length += ComputeDistance(
            {bus->stops_[i-1]->latitude_, bus->stops_[i-1]->longitude_},
            {bus->stops_[i]->latitude_, bus->stops_[i]->longitude_}
        );
    }
    return length;
}

double TransportCatalogue::CalculateRoadLength(const Bus* bus) const {
    if (!bus || bus->stops_.size() < 2) return 0.0;
    
    double length = 0.0;
    for (size_t i = 1; i < bus->stops_.size(); ++i) {
        length += GetDistanceBetweenStops(bus->stops_[i-1], bus->stops_[i]);
    }
    return length;
}

size_t TransportCatalogue::CountUniqueStops(const Bus* bus) const {
    if (!bus) return 0;
    
    std::unordered_set<const Stop*> unique_stops;
    unique_stops.reserve(bus->stops_.size());
    
    for (const Stop* stop : bus->stops_) {
        unique_stops.insert(stop);
    }
    
    return unique_stops.size();
}

BusStats TransportCatalogue::GetBusStats(std::string_view bus_name) const {
    const Bus* bus = GetBus(bus_name);
    if (!bus) {
        return {0, 0, 0.0, 0.0};
    }
    
    double road_length = CalculateRoadLength(bus);
    double geo_length = CalculateGeographicLength(bus);
    size_t unique_stops = CountUniqueStops(bus);
    
    return {
        bus->stops_.size(),
        unique_stops,
        road_length,
        geo_length > 0 ? road_length / geo_length : 0.0
    };
}

std::set<std::string_view> TransportCatalogue::GetBusesForStop(std::string_view stop_name) const {
    const Stop* stop = GetStop(stop_name);
    if (!stop) {
        return {};
    }
    
    auto it = stop_to_buses_.find(stop);
    return it != stop_to_buses_.end() ? it->second : std::set<std::string_view>();
}

} // namespace TransportSystem

