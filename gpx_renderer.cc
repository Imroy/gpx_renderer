#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <algorithm>
#include <cmath>
#include <agg_basics.h>
#include <agg_renderer_scanline.h>
#include <agg_rendering_buffer.h>
#include <agg_pixfmt_rgb.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_primitives.h>
#include <agg_scanline_u.h>
#include <agg_path_storage.h>
#include <agg_conv_stroke.h>
#include "GPX.hh"
#include "Map.hh"

// Size of each pixel in metres
#define RESOLUTION 100

bool write_ppm(const uint8_t* buf, unsigned width, unsigned height) {
  std::cout << "P6 " << width << " " << height << " 255" << std::endl;
  std::cout.write(reinterpret_cast<const char*>(buf), width * height * 3);
  return true;
}

void draw_point(agg::rasterizer_scanline_aa<>& ras, Map::coords p, double size) {
  ras.move_to_d(p.first - size,  p.second + size);
  ras.line_to_d(p.first - size,  p.second + size);
  ras.line_to_d(p.first + size,  p.second - size);
  ras.line_to_d(p.first + size,  p.second - size);
}

void calc_line(Map::coords p1, Map::coords p2, double width, Map::coords& a, Map::coords& b, Map::coords& c, Map::coords& d) {
  double dx = p2.first - p1.first;
  double dy = p2.second - p1.second;
  double len = sqrt(dx*dx + dy*dy);

  if (len > 1e-6) {
    double len_r = 1.0 / len;
    dx = width * (p2.second - p1.second) * len_r;
    dy = width * (p2.first - p1.first) * len_r;
  } else {
    dx = dy = width;
  }

  a = std::make_pair<double, double>(p1.first - dx,  p1.second + dy);
  b = std::make_pair<double, double>(p2.first - dx,  p2.second + dy);
  c = std::make_pair<double, double>(p2.first + dx,  p2.second - dy);
  d = std::make_pair<double, double>(p1.first + dx,  p1.second - dy);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << argv[0] << " <GPX file> [<GPX file>...]" << std::endl;
    exit(1);
  }

  GPX::Parser parser;
  for (int i = 1; i < argc; i++) {
    try {
      std::cerr << "Parsing file \"" << argv[i] << "\"..." << std::endl;
      parser.parse_file(argv[i]);
    }
    catch(const xmlpp::exception& ex) {
      std::cerr << "libxml++ exception: " << ex.what() << std::endl;
    }
  }

  std::cerr << "Points:\t" << parser.point_count() << std::endl;
  std::cerr << "Lattitude:\t" << parser.min_lat() << " .. " << parser.max_lat() << ", avg: " << parser.avg_lat() << std::endl;
  std::cerr << "Longitude:\t" << parser.min_lon() << " .. " << parser.max_lon() << ", avg: " << parser.avg_lon() << std::endl;

  double biggest_lat = std::min(std::abs(parser.min_lat()), std::abs(parser.max_lat()));
  std::cerr << "Biggest lat = " << biggest_lat << std::endl;

  double max_radius = 6371000 * std::cos(biggest_lat * M_PI / 180.0);
  std::cerr << "Max radius = " << max_radius << " m" << std::endl;

  double res = 2 * M_PI * max_radius / (360.0 * RESOLUTION);
  std::cerr << "Resolution = " << res << " pixels/degree" << std::endl;

  Map map(parser.min_lon(), parser.max_lat(), res);
  Map::coords image_size = map(parser.max_lon(), parser.min_lat());

  unsigned int image_width = std::ceil(image_size.first);
  unsigned int image_height = std::ceil(image_size.second);
  std::cerr << "Image will be " << image_width << " x " << image_height << std::endl;

  uint8_t *buffer = new uint8_t[image_width * image_height * 3];

  agg::rendering_buffer rbuf(buffer, image_width, image_height, image_width * 3);
  agg::pixfmt_rgb24 pixf(rbuf);
  agg::renderer_base<agg::pixfmt_rgb24> ren(pixf);
  ren.clear(agg::rgba8(0,0,0));

  agg::scanline_u8 sl;
  agg::rasterizer_scanline_aa<> ras;
  ras.gamma(agg::gamma_power(2.2));

  for (auto track_i = parser.tracks_cbegin(); track_i != parser.tracks_cend(); track_i++) {
    GPX::trk::ptr track = *track_i;

    for (auto segment_i = track->segments_cbegin(); segment_i != track->segments_cend(); segment_i++) {
      GPX::trkseg::ptr segment = *segment_i;
      if (segment->num_points() == 0)
	continue;

      agg::path_storage ps;
      agg::conv_stroke<agg::path_storage> pg(ps);
      pg.width(1.0);

      if (segment->num_points() == 1) {
	GPX::trkpt::ptr point = segment->first_point();
	draw_point(ras, map(point->lon(), point->lat()), 1.0);
      } else {
	std::list<Map::coords> outline;
	bool first = true, second = true;
	Map::coords last_point, last_b, last_c;
	for (auto point_i = segment->points_cbegin(); point_i != segment->points_cend(); point_i++) {
	  GPX::trkpt::ptr point = *point_i;
	  Map::coords p = map(point->lon(), point->lat());
	  Map::coords a, b, c, d;
	  if (first)
	    first = false;
	  else {
	    calc_line(last_point, p, 1.0, a, b, c, d);
	    if (second)
	      second = false;
	    else {
	      a.first = (a.first + last_b.first) * 0.5;
	      a.second = (a.second + last_b.second) * 0.5;
	      d.first = (d.first + last_c.first) * 0.5;
	      d.second = (d.second + last_c.second) * 0.5;
	    }
	    outline.push_back(a);
	    outline.push_back(b);
	    outline.push_front(d);
	    outline.push_front(c);
	    last_b = b;
	    last_c = c;
	  }
	  last_point = p;
	}

	first = true;
	for (auto p : outline) {
	  if (first) {
	    ras.move_to_d(p.first, p.second);
	    first = false;
	  } else
	    ras.line_to_d(p.first, p.second);
	}
      }
      ras.add_path(pg);
      agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(255, 255, 255, 255));
    }
  }

  write_ppm(buffer, image_width, image_height);

  delete [] buffer;

  return 0;
}
