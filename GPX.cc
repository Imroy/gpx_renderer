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
#include <boost/lexical_cast.hpp>
#include "GPX.hh"

namespace GPX {
  Parser::Parser() :
    xmlpp::SaxParser(),
    _lat(0.0), _lon(0.0), _hdop(0.0),
    _point_count(0)
  {}

  Parser::~Parser() {
  }

  void Parser::on_start_document() {
  }

  void Parser::on_end_document() {
  }

  void Parser::on_start_element(const Glib::ustring& name, const AttributeList& properties) {
    _context.push(name);

    if (name == "trk") {
      new_track();
      return;
    }

    if (name == "trkseg") {
      last_track()->new_segment();
    }

    if (name == "trkpt") {
      for (auto prop : properties) {
	if (prop.name == "lat")
	  _lat = boost::lexical_cast<double>(prop.value);
	if (prop.name == "lon")
	  _lon = boost::lexical_cast<double>(prop.value);
      }
    }
  }

  void Parser::on_end_element(const Glib::ustring& name) {
    if (name == "trkpt") {
      last_track()->last_segment()->new_point(_lat, _lon);

      if (_point_count == 0) {
	_tot_lat = _min_lat = _max_lat = _lat;
	_tot_lon = _min_lon = _max_lon = _lon;
      } else {
	_tot_lat += _lat;
	_tot_lon += _lon;

	if (_lat < _min_lat)
	  _min_lat = _lat;
	else if (_lat > _max_lat)
	  _max_lat = _lat;
	if (_lon < _min_lon)
	  _min_lon = _lon;
	else if (_lon > _max_lon)
	  _max_lon = _lon;
      }

      _point_count++;

      _lat = _lon = _hdop = 0;
    }

    _context.pop();
  }

  void Parser::on_characters(const Glib::ustring& characters) {
    if (_context.top() == "hdop")
      _hdop = boost::lexical_cast<double>(characters);
  }

  void Parser::on_comment(const Glib::ustring& text) {
  }

  void Parser::on_warning(const Glib::ustring& text) {
  }

  void Parser::on_error(const Glib::ustring& text) {
  }

  void Parser::on_fatal_error(const Glib::ustring& text) {
  }
}
