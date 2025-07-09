#pragma once 

#include "transport_catalogue.h" 
#include <algorithm> 
#include <vector> 
#include <iostream>

namespace TransportSystem::Stat { 

// Получить информацию об остановке 
void query_stop(TransportCatalogue& catalogue, std::string_view stop_name, std::ostream& output); 

// Обработать транспортный запрос 
void query_(TransportCatalogue& catalogue, std::string_view str, std::ostream& output); 

// Вывести статистику системы 
void output_(TransportCatalogue& catalogue, std::istream& input, std::ostream& output); 

} // namespace TransportSystem::Stat 
