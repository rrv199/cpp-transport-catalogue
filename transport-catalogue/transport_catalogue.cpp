#include "transport_catalogue.h"  
#include <execution>             
namespace TransportSystem {
// Метод добавления новой остановки в каталог
void TransportCatalogue::add_stop(Stop&& stop_) {
    stops_.push_back(std::move(stop_));                // Добавление остановки в вектор с использованием move-семантики
    Stop* stop_buf = &stops_.back();                   // Получение указателя на последнюю добавленную остановку
    stopname_to_stop.insert(StopMap::value_type(stop_buf->name_, stop_buf));  // Добавление связи имя-указатель в карту
}

// Метод добавления нового автобуса в каталог
void TransportCatalogue::add_bus(Bus&& bus_) {
    Bus* bus_buf;
    buses_.push_back(std::move(bus_));                 // Добавление автобуса в вектор с использованием move-семантики
    bus_buf = &buses_.back();                         // Получение указателя на последний добавленный автобус
    busname_to_bus.insert(BusMap::value_type(bus_buf->name_, bus_buf));  // Добавление связи имя-указатель в карту
    
    // Обновление списка автобусов для каждой остановки маршрута
    for (Stop* _stop : bus_buf->stops_) {
        _stop->buses_.push_back(bus_buf);
    }
}

// Метод поиска автобуса по имени
Bus* TransportCatalogue::get_bus(std::string_view _bus_name) {
    if(busname_to_bus.empty())  // Проверка на пустоту карты
        return nullptr;

    try {
        return busname_to_bus.at(_bus_name);  // Попытка получить автобус по имени
    } catch(const std::out_of_range &e) {
        return nullptr;  // Возврат nullptr при отсутствии автобуса
    }
}

// Метод поиска остановки по имени
Stop* TransportCatalogue::get_stop(std::string_view _stop_name) {
    if(stopname_to_stop.empty())  // Проверка на пустоту карты
        return nullptr;

    try {
        return stopname_to_stop.at(_stop_name);  // Попытка получить остановку по имени
    } catch(const std::out_of_range &e) {
        return nullptr;  // Возврат nullptr при отсутствии остановки
    }
}

// Метод получения уникальных остановок маршрута автобуса
std::unordered_set<const Stop*> TransportCatalogue::get_uniq_stops(Bus* bus) {
    std::unordered_set<const Stop*> unique_stops_;  // Создание множества для хранения уникальных остановок
    unique_stops_.insert(bus->stops_.begin(), bus->stops_.end());  // Добавление всех остановок в множество
    return unique_stops_;
}

// Метод расчета общей длины маршрута автобуса
double TransportCatalogue::get_length(Bus* bus) {
    return transform_reduce(
        next(bus->stops_.begin()),  // Начало итерации со второй остановки
        bus->stops_.end(),          // Конец последовательности
        bus->stops_.begin(),        // Начало последовательности
        0.0,                       // Начальное значение суммы
        std::plus<>{},             // Операция сложения
        [](const Stop* lhs, const Stop* rhs) {  // Лямбда-функция для вычисления расстояния
            return ComputeDistance(
                {(*lhs).latitude_, (*lhs).longitude_},  // Координаты первой точки
                {(*rhs).latitude_, (*rhs).longitude_}   // Координаты второй точки
            );
        }
    );
}

// Метод получения уникальных автобусов, проходящих через остановку
std::unordered_set<const Bus*> TransportCatalogue::stop_get_uniq_buses(Stop* stop) {
    std::unordered_set<const Bus*> unique_stops_;  // Создание множества для хранения уникальных автобусов
    unique_stops_.insert(stop->buses_.begin(), stop->buses_.end());  // Добавление всех автобусов в множество
    return unique_stops_;
}
}
