#Note that this expects your SFML libraries and headers to be
#installed as necessary

#Also asssumes that bullet physiscs is installed in common path.

#-Wno-switch to prevent the compiler from complaining about not 
#including all possible window events in switch statement.
CXXFLAGS = -g -Wall -Wno-switch -I/usr/local/include/bullet 
LIBS = -lsfml-network \
	-lsfml-window \
	-lsfml-graphics \
	-lsfml-system \
	-lGLU \
	-lBulletCollision \
	-lBulletDynamics \
	-lBulletCollision \
	-lLinearMath 
	
OBJS = main.o Avatar.o

all: 3dTD

3dTD: $(OBJS)
	g++ -g -Wall -o $@ $^ $(LIBS)