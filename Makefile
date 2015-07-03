PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# Libraries with pkg-config data
PKGS = libxml++-2.6

COMMON_FLAGS = -Wall -Iinclude -fopenmp -finput-charset=UTF-8 -Wformat -Wformat-security -Werror=format-security -D_FORTIFY_SOURCE=2 -fstack-protector --param ssp-buffer-size=4 -fPIE -pie
LIBS = -lm -lstdc++ -lgomp -lboost_filesystem

COMMON_FLAGS += `pkg-config --cflags $(PKGS)`
CFLAGS += $(COMMON_FLAGS)
CXXFLAGS += -std=c++11 $(COMMON_FLAGS)
LDFLAGS += -Wl,-z,relro -Wl,-z,now -fPIE -pie
LIBS += `pkg-config --libs $(PKGS)`

OBJS = $(patsubst %.cc,%.o,$(wildcard *.cc))


all: gpx_renderer

clean:
	@rm -fv .depend *.o gpx_renderer

gpx_renderer: $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LIBS) -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

depend:
	touch .depend
	makedepend -f .depend -I . 2> /dev/null

ifneq ($(wildcard .depend),)
include .depend
endif