#include "transport_catalogue.h"
namespace catalogue {
namespace transcat {

void TransportCatalogue::AddStop(const BusStop& bus_stop) {
  stops_.push_back(std::move(bus_stop));
  stops_table_.insert({stops_.back().stop_name, &stops_.back()});
}

void TransportCatalogue::AddBus(const Bus& bus) {
  buses_.push_back(std::move(bus));
  buses_table_.insert({buses_.back().bus_name, &buses_.back()});
  for (const auto& stop : buses_.back().route) {
    buses_via_stop_[stop->stop_name].insert(bus.bus_name);
  }
}

std::vector<std::string> TransportCatalogue::GetRoute(
    const std::string_view bus) const {
  std::vector<std::string> result;
  for(const auto& ptr_stop: buses_table_.at(bus)->route){
    result.push_back(ptr_stop->stop_name);
  }
  
  return result;
}

double TransportCatalogue::ComputeRouteDist(
    const std::vector<std::string>& route) const {
  double route_dist = 0;
  std::string_view lhs_stop = *route.begin();
  for (const std::string_view rhs_stop : route) {
    route_dist += ComputeDistance(stops_table_.at(lhs_stop)->coordinates,
                                  stops_table_.at(rhs_stop)->coordinates);
    lhs_stop = rhs_stop;
  }
  return route_dist;
}

size_t TransportCatalogue::ComputeUniqStops(
    const std::vector<std::string>& route) const {
  std::set<std::string_view> tmp;
  for (const std::string_view bus_stop : route) {
    tmp.insert(bus_stop);
  }
  return tmp.size();
}

size_t TransportCatalogue::ComputeStops(
    const std::vector<std::string>& route) const {
  return route.size();
}

bool TransportCatalogue::IsBusExsists(const std::string_view bus) const {
  if (buses_table_.count(bus) > 0) {
    return true;
  }
  return false;
}

bool TransportCatalogue::IsStopExists(const std::string_view stop) const {
  if (stops_table_.count(stop) > 0) {
    return true;
  }
  return false;
}

std::string TransportCatalogue::GetBusesViaStop(
    const std::string_view stop) const {
  if (stops_table_.count(stop) == 0) {
    return std::string("not found");
  }

  if (buses_via_stop_.count(stop) == 0) {
    return std::string("no buses");
  }

  std::string result;
  std::ostringstream line;
  line << "buses ";
  for (const auto& bus : buses_via_stop_.at(stop)) {
    line << bus << " ";
  }
  result = line.str();
  result.pop_back();

  return result;
}

}  // namespace transcat
}  // namespace catalogue