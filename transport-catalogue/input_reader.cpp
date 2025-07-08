#include "input_reader.h"
#include "stat_reader.h"
#include <algorithm>

namespace TransportSystem::Input {

// Разбирает строку с информацией об остановке (формат: "Stop <name>: <lat>, <lon>")
Stop split_stop(std::string str) {
    auto twopoint_pos = str.find(':');
    auto comma_pos = str.find(',');
    auto entry_length = 5; // "Stop "
    auto distance = 2;  // Длина разделителя ": " и ", "
    
    Stop _stop;
    _stop.name_ = str.substr(entry_length, twopoint_pos - entry_length);
    _stop.latitude_ = stod(str.substr(twopoint_pos + distance, comma_pos - twopoint_pos - distance));
    _stop.longitude_ = stod(str.substr(comma_pos + distance));
    
    return _stop;
}

// Разбирает строку с информацией о маршруте (формат: "Bus <name>: <stop1> > <stop2> > ...")
Bus split_bus(TransportCatalogue& catalogue, std::string_view str) {
    auto entry_length = 4; // "Bus "
    auto distance = 2;  // Длина разделителя ": " и " > "
    auto twopoint_pos = str.find(':');
    
    Bus bus;
    bus.name_ = str.substr(entry_length, twopoint_pos - entry_length);
    
    str = str.substr(twopoint_pos + distance);
    
    // Обработка кольцевого маршрута (разделитель " - ")
    auto more_pos = str.find('>');
    if (more_pos == std::string_view::npos) {
        auto tire_pos = str.find('-');
        
        while (tire_pos != std::string_view::npos) {
            bus.stops_.push_back(catalogue.get_stop(str.substr(0, tire_pos - 1)));
            str = str.substr(tire_pos + distance);
            tire_pos = str.find('-');
        }
        
        bus.stops_.push_back(catalogue.get_stop(str.substr(0, tire_pos - 1)));
        size_t size_ = bus.stops_.size() - 1;
        
        // Для некольцевого маршрута дублируем остановки в обратном порядке
        for (size_t i = size_; i > 0; i--) {
            bus.stops_.push_back(bus.stops_[i-1]);
        }
    } else {
        // Обработка некольцевого маршрута (разделитель " > ")
        while (more_pos != std::string_view::npos) {
            bus.stops_.push_back(catalogue.get_stop(str.substr(0, more_pos - 1)));
            str = str.substr(more_pos + distance);
            more_pos = str.find('>');
        }
        bus.stops_.push_back(catalogue.get_stop(str.substr(0, more_pos - 1)));
    }
    return bus;
}

// Основная функция ввода данных в транспортный каталог
void input_(TransportCatalogue& catalogue, std::istream& input) {
    std::string count;
    std::getline(input, count); // Чтение количества запросов
    
    if (!count.empty()) {
        std::string str;
        std::vector<std::string> buses;
        int amount = stoi(count);
        auto bus_distance = 3; // Длина префикса "Bus"
        
        // Чтение и обработка всех запросов
        for (int i = 0; i < amount; ++i) {
            std::getline(input, str);
            
            if (!str.empty()) {
                auto space_pos = str.find_first_not_of(' ');
                str = str.substr(space_pos);

                // Разделение остановок и маршрутов
                if (str.substr(0, bus_distance) != "Bus") {
                    catalogue.add_stop(split_stop(str));
                } else {
                    buses.push_back(str);
                }
            }
        }
        
        // Обработка маршрутов после всех остановок
        for (auto bus : buses) {
            catalogue.add_bus(split_bus(catalogue, bus));
        }
    }
}
} // namespace TransportSystem::Input
