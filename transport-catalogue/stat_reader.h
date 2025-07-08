#pragma once
#include "transport_catalogue.h"
#include <algorithm>
#include <vector>

namespace TransportSystem::Stat {

// Получить информацию об остановке
void query_stop(TransportCatalogue& catalogue, std::string_view stop_name);

// Обработать транспортный запрос
void query_(TransportCatalogue& catalogue, std::string_view str);

// Вывести статистику системы
void output_(TransportCatalogue& catalogue);

} // namespace TransportSystem::Stat
