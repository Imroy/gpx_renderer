#include <boost/lexical_cast.hpp>
#include "GPX.hh"

namespace GPX {
  trkpt::trkpt(const xmlpp::SaxParser::AttributeList& properties) {
    for (auto prop : properties) {
      if (prop.name == "lat")
	_lat = boost::lexical_cast<double>(prop.value);
      if (prop.name == "lon")
	_lon = boost::lexical_cast<double>(prop.value);
    }
  }

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
      _tracks.push_back(GPX::trk(properties));
      return;
    }

    if (name == "trkseg") {
      _tracks.back().new_segment(GPX::trkseg(properties));
    }

    if (name == "trkpt") {
      _tracks.back().last_segment().new_point(GPX::trkpt(properties));
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
