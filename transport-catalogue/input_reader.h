#pragma once
#include "transport_catalogue.h"

namespace TransportSystem::Input {

    // Разбирает строку с данными об остановке (формат: "Название: широта, долгота")
    Stop split_stop(std::string_view str);

    // Разбирает строку с описанием маршрута (формат: "Название: stop1 > stop2 ...")
    Bus split_bus(TransportCatalogue& catalogue, std::string_view str);

    // Основной метод для ввода данных в транспортный каталог
    void input_(TransportCatalogue& catalogue, std::istream& input = std::cin);

} // namespace TransportSystem::Input
