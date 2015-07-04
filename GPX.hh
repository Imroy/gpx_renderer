#pragma once
#include <memory>
#include <stack>
#include <string>
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

    typedef std::shared_ptr<trkpt> ptr;
  };

  class trkseg {
  private:
    std::list<trkpt::ptr> _points;

  public:
    trkseg(void) {}

    void new_point(double lat, double lon) { _points.push_back(std::make_shared<trkpt>(lat, lon)); }
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
    double _lat, _lon, _hdop;
    unsigned int _point_count;
    double _tot_lat, _min_lat, _max_lat;
    double _tot_lon, _min_lon, _max_lon;

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

    double min_lat(void) const { return _min_lat; }
    double max_lat(void) const { return _max_lat; }
    double avg_lat(void) const { return _point_count > 0 ? _tot_lat / _point_count : 0; }

    double min_lon(void) const { return _min_lon; }
    double max_lon(void) const { return _max_lon; }
    double avg_lon(void) const { return _point_count > 0 ? _tot_lon / _point_count : 0; }

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
