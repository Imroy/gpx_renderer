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
#include <algorithm>
#include <utility>
#include <cmath>

class Map {
public:
  typedef std::pair<double, double> coords;

private:
  coords _top_left;
  double _scale;

public:
  Map(coords tl, double scale) :
    _top_left(tl), _scale(scale)
  {}

  Map(double lon, double lat, double scale) :
    _top_left(lon, lat), _scale(scale)
  {}

  const coords operator ()(const coords& point) const;
};

//! Piece-wise addition
inline const Map::coords operator +(const Map::coords& a, const Map::coords& b) {
  return Map::coords(a.first + b.first, a.second + b.second);
}

//! Piece-wise subtraction
inline const Map::coords operator -(const Map::coords& a, const Map::coords& b) {
  return Map::coords(a.first - b.first, a.second - b.second);
}

//! Piece-wise multiplication
inline const Map::coords operator *(const Map::coords& a, const Map::coords& b) {
  return Map::coords(a.first * b.first, a.second * b.second);
}

//! Piece-wise division
inline const Map::coords operator /(const Map::coords& a, const Map::coords& b) {
  return Map::coords(a.first * (std::abs(b.first) > 1e-12 ? 1.0 / b.first : 1.0),
		     a.second * (std::abs(b.second) > 1e-12 ? 1.0 / b.second : 1.0));
}

template <typename T>
const Map::coords operator *(const Map::coords& a, T b) {
  return std::make_pair<double, double>(a.first * b, a.second * b);
}

template <typename T>
const Map::coords operator /(const Map::coords& a, T b) {
  double rb;
  if (std::abs(b) > 1e-12)
    rb = 1.0 / b;
  else
    rb = 1;
  return std::make_pair<double, double>(a.first * rb, a.second * rb);
}

inline const Map::coords min(const Map::coords& a, const Map::coords& b) {
  return Map::coords(std::min(a.first, b.first), std::min(a.second, b.second));
}

inline const Map::coords max(const Map::coords& a, const Map::coords& b) {
  return Map::coords(std::max(a.first, b.first), std::max(a.second, b.second));
}
