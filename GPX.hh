#pragma once
#include <libxml++/libxml++.h>

namespace GPX {
  class trkpt {
  private:
    double _lat, _lon;

  public:
    trkpt(double lat, double lon) :
      _lat(lat), _lon(lon)
    {}

    double lat(void) const { return _lat; }
    trkpt& set_lat(double l) { _lat = l; return *this; }
    double lon(void) const { return _lon; }
    trkpt& set_lon(double l) { _lon = l; return *this; }
  };

  class trkseg {
  private:
    std::list<trkpt> _points;

  public:
    trkseg(void) {}

    void new_point(trkpt pt) { _points.push_back(pt); }
    trkpt last_point(void) { return _points.back(); }
  };

  class trk {
  private:
    std::list<trkseg> _segments;

  public:
    trk(void) {}

    void new_segment(trkseg seg) { _segments.push_back(seg); }
    trkseg last_segment(void) { return _segments.back(); }
  };

  class Parser : public xmlpp::SaxParser {
  private:
    std::list<trk> _tracks;

  public:
    Parser();
    virtual ~Parser();

    void new_track(trk t) { _tracks.push_back(t); }
    trk last_track(void) { return _tracks.back(); }

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
