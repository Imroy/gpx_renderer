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
#include <memory>
#include <stack>
#include <string>
#include <libxml++/libxml++.h>
#include "Map.hh"

namespace GPX {
  class trkpt {
  private:
    Map::coords _coords;

  public:
    trkpt(double lat, double lon) :
      _coords(lon, lat)
    {}

    trkpt(const Map::coords& c) :
      _coords(c)
    {}

    Map::coords coords(void) const { return _coords; }
    trkpt& set_coords(const Map::coords& c) { _coords = c; return *this; }

    double lat(void) const { return _coords.second; }
    trkpt& set_lat(double l) { _coords.second = l; return *this; }

    double lon(void) const { return _coords.first; }
    trkpt& set_lon(double l) { _coords.first = l; return *this; }

    typedef std::shared_ptr<trkpt> ptr;
  };

  class trkseg {
  private:
    std::list<trkpt::ptr> _points;

  public:
    trkseg(void) {}

    void new_point(const Map::coords c) { _points.push_back(std::make_shared<trkpt>(c)); }
    trkpt::ptr first_point(void) { return _points.front(); }
    trkpt::ptr last_point(void) { return _points.back(); }

    size_t num_points(void) const { return _points.size(); }
    std::list<trkpt::ptr>::const_iterator points_cbegin() const { return _points.cbegin(); }
    std::list<trkpt::ptr>::const_iterator points_cend() const { return _points.cend(); }

    typedef std::shared_ptr<trkseg> ptr;
  };

  class trk {
  private:
    std::list<trkseg::ptr> _segments;

  public:
    trk(void) {}

    void new_segment(void) { _segments.push_back(std::make_shared<trkseg>()); }
    trkseg::ptr first_segment(void) { return _segments.front(); }
    trkseg::ptr last_segment(void) { return _segments.back(); }

    size_t num_segments(void) const { return _segments.size(); }
    std::list<trkseg::ptr>::const_iterator segments_cbegin() const { return _segments.cbegin(); }
    std::list<trkseg::ptr>::const_iterator segments_cend() const { return _segments.cend(); }

    typedef std::shared_ptr<trk> ptr;
  };

  class Parser : public xmlpp::SaxParser {
  private:
    std::stack<std::string> _context;
    std::list<trk::ptr> _tracks;
    Map::coords _coords;
    double _hdop;
    unsigned int _point_count;
    Map::coords _total, _min, _max;

  public:
    Parser();
    virtual ~Parser();

    void new_track(void) { _tracks.push_back(std::make_shared<trk>()); }
    trk::ptr first_track(void) { return _tracks.front(); }
    trk::ptr last_track(void) { return _tracks.back(); }

    size_t num_tracks(void) const { return _tracks.size(); }
    std::list<trk::ptr>::const_iterator tracks_cbegin() const { return _tracks.cbegin(); }
    std::list<trk::ptr>::const_iterator tracks_cend() const { return _tracks.cend(); }

    unsigned int point_count(void) const { return _point_count; }

    Map::coords min_coords(void) const { return _min; }
    Map::coords max_coords(void) const { return _max; }
    Map::coords avg_coords(void) const { return _point_count > 0 ? _total / _point_count : Map::coords(0,0); }

  protected:
    virtual void on_start_document();
    virtual void on_end_document();
    virtual void on_start_element(const Glib::ustring& name, const AttributeList& properties);
    virtual void on_end_element(const Glib::ustring& name);
    virtual void on_characters(const Glib::ustring& characters);
    virtual void on_comment(const Glib::ustring& text);
    virtual void on_warning(const Glib::ustring& text);
    virtual void on_error(const Glib::ustring& text);
    virtual void on_fatal_error(const Glib::ustring& text);
  };


}
