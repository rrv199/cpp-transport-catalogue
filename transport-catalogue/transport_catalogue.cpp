#include "transport_catalogue.h"

namespace TransportSystem {

// Добавляет новую остановку в каталог
void TransportCatalogue::add_stop(Stop&& stop_) {
    stops_.push_back(std::move(stop_));
    Stop* stop_ptr = &stops_.back();
    stopname_to_stop[stop_ptr->name_] = stop_ptr;
}

// Добавляет новый маршрут в каталог и обновляет информацию об остановках
void TransportCatalogue::add_bus(Bus&& bus_) {
    buses_.push_back(std::move(bus_));
    Bus* bus_ptr = &buses_.back();
    busname_to_bus[bus_ptr->name_] = bus_ptr;
    
    for (Stop* stop : bus_ptr->stops_) {
        stop_to_buses_[stop].insert(bus_ptr->name_);
    }
}

// Возвращает указатель на маршрут по имени (неконстантная версия)
Bus* TransportCatalogue::get_bus(std::string_view bus_name) {
    auto it = busname_to_bus.find(bus_name);
    return it != busname_to_bus.end() ? it->second : nullptr;
}

// Возвращает указатель на маршрут по имени (константная версия)
const Bus* TransportCatalogue::get_bus(std::string_view bus_name) const {
    auto it = busname_to_bus.find(bus_name);
    return it != busname_to_bus.end() ? it->second : nullptr;
}

// Возвращает указатель на остановку по имени (неконстантная версия)
Stop* TransportCatalogue::get_stop(std::string_view stop_name) {
    auto it = stopname_to_stop.find(stop_name);
    return it != stopname_to_stop.end() ? it->second : nullptr;
}

// Возвращает указатель на остановку по имени (константная версия)
const Stop* TransportCatalogue::get_stop(std::string_view stop_name) const {
    auto it = stopname_to_stop.find(stop_name);
    return it != stopname_to_stop.end() ? it->second : nullptr;
}

// Возвращает уникальные остановки маршрута
std::unordered_set<const Stop*> TransportCatalogue::get_uniq_stops(const Bus* bus) const {
    return {bus->stops_.begin(), bus->stops_.end()};
}

// Вычисляет географическую длину маршрута
double TransportCatalogue::get_length(const Bus* bus) const {
    return std::transform_reduce(
        std::next(bus->stops_.begin()),
        bus->stops_.end(),
        bus->stops_.begin(),
        0.0,
        std::plus<>(),
        [](const Stop* lhs, const Stop* rhs) {
            return ComputeDistance(
                {lhs->latitude_, lhs->longitude_},
                {rhs->latitude_, rhs->longitude_}
            );
        }
    );
}

// Возвращает список маршрутов, проходящих через остановку
const std::set<std::string_view>& TransportCatalogue::get_buses_for_stop(const Stop* stop) const {
    static const std::set<std::string_view> empty;
    auto it = stop_to_buses_.find(stop);
    return it != stop_to_buses_.end() ? it->second : empty;
}

BusStats TransportCatalogue::get_bus_stats(const Bus* bus) const {
    return {
        bus->stops_.size(),
        get_uniq_stops(bus).size(),
        get_length(bus)
    };
}
} // namespace TransportSystem
