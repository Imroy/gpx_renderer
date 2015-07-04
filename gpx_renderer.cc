#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <algorithm>
#include <cmath>
#include <agg_rendering_buffer.h>
#include "GPX.hh"

// Size of each pixel in metres
#define RESOLUTION 100

bool write_ppm(const uint8_t* buf, unsigned width, unsigned height) {
  std::cout << "P6 " << width << " " << height << " 255" << std::endl;
  std::cout.write(reinterpret_cast<const char*>(buf), width * height * 3);
  return true;
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

  unsigned int frame_width = std::ceil((parser.max_lon() - parser.min_lon()) * res);
  unsigned int frame_height = std::ceil((parser.max_lat() - parser.min_lat()) * res);
  std::cerr << "Image will be " << frame_width << " x " << frame_height << std::endl;

  uint8_t *buffer = new uint8_t[frame_width * frame_height * 3];
  memset(buffer, 0, frame_width * frame_height * 3);

  agg::rendering_buffer rbuf(buffer, frame_width, frame_height, frame_width * 3);

  write_ppm(buffer, frame_width, frame_height);

  delete [] buffer;

  return 0;
}
