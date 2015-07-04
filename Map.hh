#pragma once
#include <utility>

class Map {
private:
  double _min_lon, _max_lat, _scale;

public:
  Map(double min_lon, double max_lat, double scale);

  typedef std::pair<double, double> coords;
  coords operator ()(double lon, double lat);
};
