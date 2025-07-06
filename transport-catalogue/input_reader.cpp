#include "input_reader.h"
#include "stat_reader.h"
#include <algorithm>

namespace TransportSystem::Input {
// Функция для парсинга строки с информацией об остановке
Stop split_stop(std::string str) {
    // Поиск разделителей в строке
    auto twopoint_pos = str.find(':');
    auto comma_pos = str.find(',');
    auto entry_length = 5; // Длина префикса "Stop "
    auto distance = 2;     // Расстояние между разделителем и данными
    
    Stop _stop;
    
    // Извлечение названия остановки (между "Stop " и ':')
    _stop.name_ = str.substr(entry_length, twopoint_pos - entry_length);
    
    // Парсинг широты (между ':' и ',')
    _stop.latitude_ = stod(str.substr(twopoint_pos + distance, comma_pos - twopoint_pos - distance));
    
    // Парсинг долготы (после запятой)
    _stop.longitude_ = stod(str.substr(comma_pos + distance));
    
    return _stop;
}

// Функция для парсинга маршрута автобуса
Bus split_bus(TransportCatalogue& catalogue, std::string_view str) {
    auto entry_length = 4; // Длина префикса "Bus "
    auto distance = 2;     // Отступ от разделителя
    auto twopoint_pos = str.find(':');
    
    Bus bus;
    // Извлечение названия маршрута (между "Bus " и ':')
    bus.name_ = str.substr(entry_length, twopoint_pos - entry_length);
    
    str = str.substr(twopoint_pos + distance); // Обрезаем обработанную часть
    
    // Обработка двух типов маршрутов: кольцевые (с '>') и линейные (с '-')
    auto more_pos = str.find('>');
    if (more_pos == std::string_view::npos) { // Линейный маршрут
        auto tire_pos = str.find('-');
        
        // Парсим все остановки до разделителей '-'
        while (tire_pos != std::string_view::npos) {
            bus.stops_.push_back(catalogue.get_stop(str.substr(0, tire_pos - 1)));
            str = str.substr(tire_pos + distance);
            tire_pos = str.find('-');
        }
        
        // Добавляем последнюю остановку
        bus.stops_.push_back(catalogue.get_stop(str.substr(0, tire_pos - 1)));
        size_t size_ = bus.stops_.size() - 1;
        
        // Создаем обратный маршрут для линейных маршрутов
        for (size_t i = size_; i > 0; i--) {
            bus.stops_.push_back(bus.stops_[i-1]);
        }
    } else { // Кольцевой маршрут
        while (more_pos != std::string_view::npos) {
            bus.stops_.push_back(catalogue.get_stop(str.substr(0, more_pos - 1)));
            str = str.substr(more_pos + distance);
            more_pos = str.find('>');
        }
        bus.stops_.push_back(catalogue.get_stop(str.substr(0, more_pos - 1)));
    }
    return bus;
}

// Основная функция обработки ввода
void input_(TransportCatalogue& catalogue) {
    std::string count;
    std::getline(std::cin, count); // Чтение количества записей
    
    if (count != "") {
        std::string str;
        std::vector<std::string> buses; // Буфер для маршрутов
        int amount = stoi(count);
        auto bus_distance = 3; // Длина префикса "Bus"
        
        // Первый проход: обработка всех остановок
        for (int i = 0; i < amount; ++i) {
            std::getline(std::cin, str);
            
            if (str != "") {
                auto space_pos = str.find_first_not_of(' ');
                str = str.substr(space_pos); // Удаление пробелов в начале

                if (str.substr(0, bus_distance) != "Bus") {
                    catalogue.add_stop(split_stop(str)); // Добавление остановки
                } else {
                    buses.push_back(str); // Сохранение маршрута для последующей обработки
                }
            }
        }
        
        // Второй проход: обработка маршрутов (после всех остановок)
        for (auto bus : buses) {
            catalogue.add_bus(split_bus(catalogue, bus));
        }
    }
}
}
