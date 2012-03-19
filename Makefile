#-Wno-switch to prevent the compiler from complaining about not 
#including all possible window events in switch statement.
#
#Last includes are so OGLFT can be configured property
CXXFLAGS = -g -Wall -Wno-switch -I/usr/include/freetype2 -I/usr/include/qt4/Qt -I/usr/include/qt4

#Note that this expects your SFML libraries and headers to be
#installed as necessary

#This requires tha OGLFT is installed from:
#http://oglft.sourceforge.net/
#
#To get it working you also need libgle3 from repo and libqt4-dev

#Assumes assimp is installed on search path. Used for awesome
#matrix math libaries 
LIBS = -lsfml-network \
	-lsfml-window \
	-lsfml-graphics \
	-lsfml-system \
	-lassimp \
	-lOGLFT \
	-lGLU \
	-lGLEW 
	
OBJS = main.o Avatar.o Camera.o Shader.o HardwareParticleSystem.o Maze.o MazeTile.o TileData.o Turret.o FireWheel.o TurretFactory.o \
	Lightning.o GravityRain.o Creep.o CreepManager.o 

all: 3dTD

3dTD: $(OBJS)
	g++ -g -Wall -o $@ $^ $(LIBS)
	
clean: 
	rm -f *.o