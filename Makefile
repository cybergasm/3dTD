#-Wno-switch to prevent the compiler from complaining about not 
#including all possible window events in switch statement.
CXXFLAGS = -g -Wall -Wno-switch -I/usr/local/include/bullet 

#Note that this expects your SFML libraries and headers to be
#installed as necessary

#Asssumes that bullet physiscs is installed in common path.

#Assumes assimp is installed on search path. Used for awesome
#matrix math libaries 
LIBS = -lsfml-network \
	-lsfml-window \
	-lsfml-graphics \
	-lsfml-system \
	-lBulletCollision \
	-lBulletDynamics \
	-lBulletCollision \
	-lLinearMath \
	-lassimp \
	-lGLU \
	-lGLEW 
	
OBJS = main.o Avatar.o Camera.o Shader.o HardwareParticleSystem.o Maze.o MazeTile.o TileData.o Turret.o FireWheel.o TurretFactory.o \
	Lightning.o

all: 3dTD

3dTD: $(OBJS)
	g++ -g -Wall -o $@ $^ $(LIBS)
	
clean: 
	rm -f *.o