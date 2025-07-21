
#pragma once
#include "transport_catalogue.h"
#include <iostream>

namespace TransportSystem::Stat {

void ProcessQueries(const TransportCatalogue& catalogue, std::istream& input, std::ostream& output);

} // namespace TransportSystem::Stat
