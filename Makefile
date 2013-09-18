TARGET=vaders

CXX=clang
CXXFLAGS=-Wno-unknown-pragmas
CPPFLAGS=-g
LDFLAGS=
LDLIBS=-lSDL_image -lSDL_gfx

SRCS=vaders.c util.c globals.c input.c player.c enemy.c bullet.c images.c collider.c
INCLUDES=vaders.h util.h globals.h input.h player.h enemy.h bullet.h images.h collider.h
OBJS=$(subst .c,.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) `sdl-config --cflags --libs`

$(SRCS):
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -I$(INCLUDES)

clean:
	rm -rf *.o $(TARGET)

install:
	sudo cp $(TARGET) /usr/bin/$(TARGET)
