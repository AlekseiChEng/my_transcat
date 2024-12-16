#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"
namespace catalogue {
namespace streader
{
void ParseAndPrintStat(const transcat::TransportCatalogue& tansport_catalogue, std::string_view request,
                       std::ostream& output);

}
}