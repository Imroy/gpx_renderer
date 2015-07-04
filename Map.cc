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
#include "Map.hh"

Map::Map(double min_lon, double max_lat, double scale) :
  _min_lon(min_lon), _max_lat(max_lat), _scale(scale)
{}

Map::coords Map::operator ()(double lon, double lat) {
  return std::make_pair<double>((lon - _min_lon) * _scale, (_max_lat - lat) * _scale);
}
