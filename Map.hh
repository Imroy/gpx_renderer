/*
	Copyright 2015 Ian Tester

	This file is part of GPX Renderer.

	Photo Finish is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Photo Finish is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Photo Finish.  If not, see <http://www.gnu.org/licenses/>.
*/
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
