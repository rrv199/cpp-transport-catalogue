#include "stat_reader.h"
#include <iomanip>
#include <algorithm>
#include <vector>

namespace TransportSystem::Stat {

// Обрабатывает запрос информации о маршруте (формат: "Bus <имя маршрута>")
void query_bus(TransportCatalogue& catalogue, std::string_view str) {
    auto entry = 4; // Длина префикса "Bus "
    str = str.substr(entry);
    
    if (Bus* bus = catalogue.get_bus(str)) {
        auto unique_stops = catalogue.get_uniq_stops(bus);
        std::cout << "Bus " << bus->name_ << ": " 
               << bus->stops_.size() << " stops on route, "
               << unique_stops.size() << " unique stops, "
               << std::setprecision(6) << catalogue.get_length(bus) << " route length" << std::endl;
    } else {
        std::cout << "Bus " << str << ": not found" << std::endl;
    }
}

// Обрабатывает запрос информации об остановке (формат: "Stop <имя остановки>")
void query_stop(TransportCatalogue& catalogue, std::string_view stop_name) {
    auto entry = 5; // Длина префикса "Stop "
    stop_name = stop_name.substr(entry);
    
    if (Stop* stop = catalogue.get_stop(stop_name)) {
        const auto& buses = catalogue.get_buses_for_stop(stop);
        
        if (buses.empty()) {
            std::cout << "Stop " << stop_name << ": no buses" << std::endl;
        } else {
            std::cout << "Stop " << stop_name << ": buses";
            for (const auto& bus_name : buses) {
                std::cout << " " << bus_name;
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Stop " << stop_name << ": not found" << std::endl;
    }
}

// Определяет тип запроса и перенаправляет на соответствующий обработчик
void query_(TransportCatalogue& catalogue, std::string_view str) {
    if (str.substr(0, 3) == "Bus") {
        query_bus(catalogue, str);
    } else if (str.substr(0, 4) == "Stop") {
        query_stop(catalogue, str);
    } else {
        std::cout << "Error query" << std::endl;
    }
}

// Основная функция вывода статистики (читает запросы и выводит результаты)
void output_(TransportCatalogue& catalogue) {
    std::string count;
    std::getline(std::cin, count); // Чтение количества запросов
    std::string str;
    std::vector<std::string> query;
    auto amount = stoi(count);
    
    // Чтение всех запросов
    for (int i = 0; i < amount; ++i) {
        std::getline(std::cin, str);
        query.push_back(str);
    }
    
    // Обработка и вывод результатов для каждого запроса
    for (auto& strr : query) {
        query_(catalogue, strr);
    }
}
} // namespace TransportSystem::Stat
