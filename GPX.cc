#include <boost/lexical_cast.hpp>
#include "GPX.hh"

namespace GPX {
  Parser::Parser() :
    xmlpp::SaxParser()
  {}

  Parser::~Parser() {
  }

  void Parser::on_start_document() {
  }

  void Parser::on_end_document() {
  }

  void Parser::on_start_element(const Glib::ustring& name, const AttributeList& properties) {
    if (name == "trk") {
      new_track(GPX::trk());
      return;
    }

    if (name == "trkseg") {
      last_track().new_segment(GPX::trkseg());
    }

    if (name == "trkpt") {
      double lat, lon;
      for (auto prop : properties) {
	if (prop.name == "lat")
	  lat = boost::lexical_cast<double>(prop.value);
	if (prop.name == "lon")
	  lon = boost::lexical_cast<double>(prop.value);
      }
      last_track().last_segment().new_point(GPX::trkpt(lat, lon));
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
