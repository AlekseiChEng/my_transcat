#include "stat_reader.h"

#include <iomanip>
#include <iostream>
#include <string>

#include "input_reader.h"

namespace catalogue {
namespace streader {
using namespace std::literals;

namespace detail {

std::string_view Trim(std::string_view string) {
  const auto start = string.find_first_not_of(' ');
  if (start == string.npos) {
    return {};
  }
  return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

std::vector<std::string_view> StatSplit(std::string_view string, char delim) {
  std::vector<std::string_view> result;

  size_t pos = 0;
  while ((pos = string.find_first_not_of(' ', pos)) < string.length()) {
    auto delim_pos = string.find(delim, pos);
    if (delim_pos == string.npos) {
      delim_pos = string.size();
    }
    if (auto substr = Trim(string.substr(pos, delim_pos - pos));
        !substr.empty()) {
      result.push_back(substr);
    }
    pos = delim_pos + 1;
  }

  return result;
}

std::string_view CutFirstWord(std::string_view string) {
  auto space_pos = string.find(' ');
  return std::string_view(string.substr(space_pos));
}
}  // namespace detail

void ParseAndPrintStat(const transcat::TransportCatalogue& transport_catalogue,
                       std::string_view request, std::ostream& output) {
  std::string_view req_type =
      detail::Trim(request.substr(0, request.find(' ')));
  std::string_view req_argument = detail::Trim(detail::CutFirstWord(request));

  if (req_type == "Stop"sv) {
    output << "Stop "sv << req_argument << ": "sv;

    output << transport_catalogue.GetBusesViaStop(req_argument) << std::endl;

  } else if (req_type == "Bus"sv) {
    if (!transport_catalogue.IsBusExsists(req_argument)) {
      output << "Bus "sv << req_argument << ": "sv << "not found"sv
             << std::endl;
    } else {
      size_t uniq_bus_stops = transport_catalogue.ComputeUniqStops(
          transport_catalogue.GetRoute(req_argument));

      size_t num_bus_stops = transport_catalogue.ComputeStops(
          transport_catalogue.GetRoute(req_argument));

      double route_length = transport_catalogue.ComputeRouteDist(
          transport_catalogue.GetRoute(req_argument));

      output << "Bus "sv << req_argument << ": "sv << num_bus_stops
             << " stops on route, "sv << uniq_bus_stops << " unique stops, "sv
             << std::setprecision(6) << route_length << " route length"sv
             << "\n"sv;
    }
  }
}
}  // namespace streader
}  // namespace catalogue
