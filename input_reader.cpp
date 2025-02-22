#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>
namespace catalogue {
namespace inpreader {

using namespace std::literals;

namespace detail {

Coordinates ParseCoordinates(std::string_view str) {
  static const double nan = std::nan("");

  auto not_space = str.find_first_not_of(' ');
  auto comma = str.find(',');

  if (comma == str.npos) {
    return {nan, nan};
  }

  auto not_space2 = str.find_first_not_of(' ', comma + 1);

  double lat = std::stod(std::string(str.substr(not_space, comma - not_space)));
  double lng = std::stod(std::string(str.substr(not_space2)));

  return {lat, lng};
}

std::string_view Trim(std::string_view string) {
  const auto start = string.find_first_not_of(' ');
  if (start == string.npos) {
    return {};
  }
  return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

std::vector<std::string_view> Split(std::string_view string, char delim) {
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

std::vector<std::string_view> ParseRoute(std::string_view route) {
  if (route.find('>') != route.npos) {
    return Split(route, '>');
  }

  auto stops = Split(route, '-');
  std::vector<std::string_view> results(stops.begin(), stops.end());
  results.insert(results.end(), std::next(stops.rbegin()), stops.rend());

  return results;
}

CommandDescription ParseCommandDescription(std::string_view line) {
  auto colon_pos = line.find(':');
  if (colon_pos == line.npos) {
    return {};
  }

  auto space_pos = line.find(' ');
  if (space_pos >= colon_pos) {
    return {};
  }

  auto not_space = line.find_first_not_of(' ', space_pos);
  if (not_space >= colon_pos) {
    return {};
  }

  return {std::string(line.substr(0, space_pos)),
          std::string(line.substr(not_space, colon_pos - not_space)),
          std::string(line.substr(colon_pos + 1))};
}
}  // namespace detail
void InputReader::ParseLine(std::string_view line) {
  auto command_description = detail::ParseCommandDescription(line);
  if (command_description) {
    commands_.push_back(std::move(command_description));
  }
}

void inpreader::InputReader::ApplyCommands(
    transcat::TransportCatalogue& catalogue) const {
  for (const auto& command_description : commands_) {
    if (command_description.command == "Stop"sv) {
      transcat::BusStop bus_stop;
      bus_stop.stop_name = command_description.id;
      bus_stop.coordinates = detail::ParseCoordinates(
          detail::Trim(command_description.description));
      catalogue.AddStop(bus_stop);
    }
  }

  for (const auto& command_description : commands_) {
    if (command_description.command == "Bus"sv) {
      transcat::Bus bus;
      bus.bus_name = command_description.id;
      std::vector<std::string_view> tmp =
          (detail::ParseRoute(command_description.description));
      for (std::string_view& stop : tmp) {
        const catalogue::transcat::BusStop* ptr_stop =
            catalogue.GetStopPtr(stop);
        bus.route.push_back(catalogue.GetStopPtr(stop));
      }
      catalogue.AddBus(bus);
    }
  }
}

}  // namespace inpreader
}  // namespace catalogue