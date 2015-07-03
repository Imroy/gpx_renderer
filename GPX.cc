#include <boost/lexical_cast.hpp>
#include "GPX.hh"

namespace GPX {
  Parser::Parser() :
    xmlpp::SaxParser(),
    _point_count(0)
  {}

  Parser::~Parser() {
  }

  void Parser::on_start_document() {
  }

  void Parser::on_end_document() {
  }

  void Parser::on_start_element(const Glib::ustring& name, const AttributeList& properties) {
    if (name == "trk") {
      new_track();
      return;
    }

    if (name == "trkseg") {
      last_track()->new_segment();
    }

    if (name == "trkpt") {
      double lat = 0, lon = 0;
      for (auto prop : properties) {
	if (prop.name == "lat")
	  lat = boost::lexical_cast<double>(prop.value);
	if (prop.name == "lon")
	  lon = boost::lexical_cast<double>(prop.value);
      }
      if (_point_count == 0) {
	_tot_lat = _min_lat = _max_lat = lat;
	_tot_lon = _min_lon = _max_lon = lon;
      } else {
	_tot_lat += lat;
	_tot_lon += lon;

	if (lat < _min_lat)
	  _min_lat = lat;
	else if (lat > _max_lat)
	  _max_lat = lat;
	if (lon < _min_lon)
	  _min_lon = lon;
	else if (lon > _max_lon)
	  _max_lon = lon;
      }
      _point_count++;
      last_track()->last_segment()->new_point(lat, lon);
    }
  }

  void Parser::on_end_element(const Glib::ustring& name) {
  }

  void Parser::on_characters(const Glib::ustring& characters) {
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
