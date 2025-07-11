#include "stat_reader.h" 
#include <iomanip> 
#include <algorithm> 
#include <vector> 

namespace TransportSystem::Stat { 

// Обрабатывает запрос информации о маршруте (формат: "Bus <имя маршрута>") 
void query_bus(TransportCatalogue& catalogue, std::string_view str, std::ostream& output) {  
    auto entry = 4; // Длина префикса "Bus "  
    str = str.substr(entry);  
      
    if (Bus* bus = catalogue.get_bus(str)) {  
        const auto stats = catalogue.get_bus_stats(bus);
        output << "Bus " << bus->name_ << ": "   
               << stats.stops_count << " stops on route, "  
               << stats.unique_stops_count << " unique stops, "  
               << std::setprecision(6) << stats.route_length << " route length" << std::endl;  
    } else {  
        output << "Bus " << str << ": not found" << std::endl;  
    }  
}  

// Обрабатывает запрос информации об остановке (формат: "Stop <имя остановки>") 
void query_stop(TransportCatalogue& catalogue, std::string_view stop_name, std::ostream& output) { 
    auto entry = 5; // Длина префикса "Stop " 
    stop_name = stop_name.substr(entry); 
     
    if (Stop* stop = catalogue.get_stop(stop_name)) { 
        const auto& buses = catalogue.get_buses_for_stop(stop); 
         
        if (buses.empty()) { 
            output << "Stop " << stop_name << ": no buses" << std::endl; 
        } else { 
            output << "Stop " << stop_name << ": buses"; 
            for (const auto& bus_name : buses) { 
                output << " " << bus_name; 
            } 
            output << std::endl; 
        } 
    } else { 
        output << "Stop " << stop_name << ": not found" << std::endl; 
    } 
} 

// Определяет тип запроса и перенаправляет на соответствующий обработчик 
void query_(TransportCatalogue& catalogue, std::string_view str, std::ostream& output) { 
    if (str.substr(0, 3) == "Bus") { 
        query_bus(catalogue, str, output); 
    } else if (str.substr(0, 4) == "Stop") { 
        query_stop(catalogue, str, output); 
    } else { 
        output << "Error query" << std::endl; 
    } 
} 

// Основная функция вывода статистики (читает запросы и выводит результаты) 
void output_(TransportCatalogue& catalogue, std::istream& input, std::ostream& output) { 
    std::string count; 
    std::getline(input, count); // Чтение количества запросов 
    std::string str; 
    std::vector<std::string> query; 
    auto amount = stoi(count); 
     
    // Чтение всех запросов 
    for (int i = 0; i < amount; ++i) { 
        std::getline(input, str); 
        query.push_back(str); 
    } 
     
    // Обработка и вывод результатов для каждого запроса 
    for (auto& strr : query) { 
        query_(catalogue, strr, output); 
    } 
} 

} // namespace TransportSystem::Stat 
