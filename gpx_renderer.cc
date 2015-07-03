#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <boost/lexical_cast.hpp>
#include "agg2/agg_rendering_buffer.h"
#include "GPX.hh"

bool write_ppm(const uint8_t* buf, unsigned width, unsigned height) {
  std::cout << "P6 " << width << " " << height << " 255 " << std::endl;
  std::cout.write(reinterpret_cast<const char*>(buf), width * height * 3);
  return true;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << argv[0] << " <width> <height>" << std::endl;
    exit(1);
  }

  unsigned int frame_width, frame_height;
  frame_width = boost::lexical_cast<unsigned int>(argv[1]);
  frame_height = boost::lexical_cast<unsigned int>(argv[2]);

  uint8_t *buffer = new uint8_t[frame_width * frame_height * 3];
  memset(buffer, 0, frame_width * frame_height * 3);

  agg::rendering_buffer rbuf(buffer, frame_width, frame_height, frame_width * 3);

  write_ppm(buffer, frame_width, frame_height);

  delete [] buffer;
  return 0;
}
