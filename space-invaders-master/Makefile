TARGET=spaceinvaders

CXX=clang
CXXFLAGS=-Wno-unknown-pragmas
CPPFLAGS=-g
LDFLAGS=
LDLIBS=-lSDL_image -lSDL_gfx -lSDL_ttf

SRC=$(wildcard *.c)
INCLUDE=$(wildcard *.h)
OBJ=$(SRC:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS) `sdl-config --cflags --libs`

$(SRC):
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -I$(INCLUDE)

clean:
	rm -rf *.o $(TARGET)

install:
	sudo cp $(TARGET) /usr/bin/$(TARGET)

fclean:		clean
		$(RM) $(TARGET)

re:		fclean all