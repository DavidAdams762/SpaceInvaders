TARGET=vaders

CXX=clang
CXXFLAGS=-Wno-unknown-pragmas
CPPFLAGS=-g
LDFLAGS=
LDLIBS=-lSDL_image -lSDL_gfx

SRCS=$(wildcard *.c)
INCLUDES=$(wildcard *.h)
OBJS=$(SRCS:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS) `sdl-config --cflags --libs`

$(SRCS):
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -I$(INCLUDES)

clean:
	rm -rf *.o $(TARGET)

install:
	sudo cp $(TARGET) /usr/bin/$(TARGET)