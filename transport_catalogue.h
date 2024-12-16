#pragma once

#include <deque>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "geo.h"
namespace catalogue {
namespace transcat {

struct BusStop {
  std::string stop_name;
  Coordinates coordinates;
};

struct Bus {
  std::string bus_name;
  std::vector<std::string> route;
};

class TransportCatalogue {
 public:
  void AddStop(const BusStop& bus_stop);

  void AddBus(const Bus& bus);

  std::vector<std::string> GetRoute(const std::string_view bus) const;

  double ComputeRouteDist(const std::vector<std::string>& route) const;

  size_t ComputeUniqStops(const std::vector<std::string>& route) const;

  size_t ComputeStops(const std::vector<std::string>& route) const;

  bool IsBusExsists(const std::string_view bus) const;

  bool IsStopExists(const std::string_view stop) const;

  std::string GetBusesViaStop(const std::string_view stop) const;

 private:
  std::deque<BusStop> stops_;
  std::unordered_map<std::string_view, const BusStop*> stops_table_;
  std::unordered_map<std::string_view, std::set<std::string>> buses_via_stop_;
  std::deque<Bus> buses_;
  std::unordered_map<std::string_view, Bus*> buses_table_;
};
}  // namespace transcat
}  // namespace catalogue