
#pragma once
#include "transport_catalogue.h"
#include <string_view>

namespace TransportSystem::Input {

void ParseAndLoadData(TransportCatalogue& catalogue, std::istream& input);

} // namespace TransportSystem::Input
