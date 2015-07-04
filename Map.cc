#include "Map.hh"

Map::Map(double min_lon, double max_lat, double scale) :
  _min_lon(min_lon), _max_lat(max_lat), _scale(scale)
{}

Map::coords Map::operator ()(double lon, double lat) {
  return std::make_pair<double>((lon - _min_lon) * _scale, (_max_lat - lat) * _scale);
}
