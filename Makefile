#Note that this expects your SFML libraries and headers to be
#installed as necessary
LIBS = -lsfml-network \
	-lsfml-window \
	-lsfml-graphics \
	-lsfml-system \
	-lGLU
	
OBJS = main.o

all: 3dTD

3dTD: $(OBJS)
	g++ -g -Wall -o $@ $^ $(LIBS)