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
#include "geo.h"

namespace TransportSystem {

struct Bus;

// Структура описывающая остановку
struct Stop {
    std::string name_;      // Название остановки
    double latitude_;       // Географическая широта
    double longitude_;      // Географическая долгота
};

// Структура описывающая маршрут
struct Bus {
    std::string name_;      // Название маршрута
    std::vector<Stop*> stops_;  // Список остановок маршрута
};

// Класс транспортного справочника
class TransportCatalogue {
public:
    // Добавляет маршрут в каталог
    void add_bus(Bus&& bus);
    
    // Добавляет остановку в каталог
    void add_stop(Stop&& stop);
    
    // Возвращает маршрут по имени (неконстантная версия)
    Bus* get_bus(std::string_view bus_name);
    
    // Возвращает маршрут по имени (константная версия)
    const Bus* get_bus(std::string_view bus_name) const;
    
    // Возвращает остановку по имени (неконстантная версия)
    Stop* get_stop(std::string_view stop_name);
    
    // Возвращает остановку по имени (константная версия)
    const Stop* get_stop(std::string_view stop_name) const;

    // Возвращает список маршрутов через указанную остановку
    const std::set<std::string_view>& get_buses_for_stop(const Stop* stop) const;
    
    // Возвращает уникальные остановки маршрута
    std::unordered_set<const Stop*> get_uniq_stops(const Bus* bus) const;
    
    // Вычисляет географическую длину маршрута
    double get_length(const Bus* bus) const;

private:
    std::deque<Stop> stops_;  // Хранилище остановок
    std::unordered_map<std::string_view, Stop*> stopname_to_stop;  // Поиск остановки по имени

    std::deque<Bus> buses_;  // Хранилище маршрутов
    std::unordered_map<std::string_view, Bus*> busname_to_bus;  // Поиск маршрута по имени

    // Хранит список маршрутов для каждой остановки
    std::unordered_map<const Stop*, std::set<std::string_view>> stop_to_buses_;
};

} // namespace TransportSystem
