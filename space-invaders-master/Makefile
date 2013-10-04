NAME=spaceinvaders

CLANG=clang
CFLAGS=-Wno-unknown-pragmas -g
LDLIBS=-lSDL_image -lSDL_gfx

SRC=$(wildcard *.c)
INCLUDE=$(wildcard *.h)
OBJ=$(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CLANG) -o $(NAME) $(OBJ) $(LDLIBS) `sdl-config --cflags --libs`

$(SRC):
	$(CLANG) $(CFLAGS) -c $< -I$(INCLUDE)

clean:
	rm -rf *.o $(TARGET)

fclean:		clean
		$(RM) $(TARGET)

re:		fclean all