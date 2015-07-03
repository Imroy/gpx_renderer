#pragma once
#include <libxml++/libxml++.h>

namespace GPX {
  class trkpt {
  private:
    double _lat, _lon;

  public:
    trkpt(const xmlpp::SaxParser::AttributeList& properties);
  };

  class trkseg {
  private:
    std::list<trkpt> _points;

  public:
    trkseg(const xmlpp::SaxParser::AttributeList& properties) {}

    void new_point(trkpt pt) { _points.push_back(pt); }
    trkpt last_point(void) { return _points.back(); }
  };

  class trk {
  private:
    std::list<trkseg> _segments;

  public:
    trk(const xmlpp::SaxParser::AttributeList& properties) {}

    void new_segment(trkseg seg) { _segments.push_back(seg); }
    trkseg last_segment(void) { return _segments.back(); }
  };

  class Parser : public xmlpp::SaxParser {
  private:
    std::list<trk> _tracks;

  public:
    Parser();
    virtual ~Parser();

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
