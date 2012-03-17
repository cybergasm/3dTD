#include <iostream>
#include <string.h>

#include <bullet/btBulletDynamicsCommon.h>

#include "Framework.h"
#include "Avatar.h"
#include "Camera.h"
#include "Shader.h"
#include "Maze.h"
#include "TurretFactory.h"
#include "Creep.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209
#endif

/**
 * Initial window parameters
 */
const int initWinHeight = 800;
const int initWinWidth = 600;

//Set-up the window settings and get a handle to a window
sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(initWinHeight, initWinWidth),
    "Tower Defense Rules!", sf::Style::Close, settings);

/**
 * Intiial camera variables and movement
 */
Camera* camera;

//We set this flag to make sure that moving the mouse around then trying
//to move does not cause jumpy motion
bool mouseReady = false;

//Projection configuration
float fov = 45;
float nearClip = .1f;
float farClip = 500.0f;

float aspect = initWinWidth / initWinWidth;

/**
 * Maze configuration
 */
Maze* maze;
string mazeString;

/**
 * Avatar configuration
 */
Avatar* avatar;

/**
 * Shaders
 */
//Shader for anything rendered as a particle system
Shader* particleSystemShader;
//Shader for simple transformation
Shader* simpleShader;
/**
 * Creeps
 */
Creep* creep;

void glInit() {
#ifdef FRAMEWORK_USE_GLEW
  GLint error = glewInit();
  if (GLEW_OK != error) {
    std::cerr << glewGetErrorString(error) << std::endl;
    exit(-1);
  }
  if (!GLEW_VERSION_2_0 || !GL_EXT_framebuffer_object) {
    std::cerr << "This program requires OpenGL 2.0 and FBOs" << std::endl;
    exit(-1);
  }
#endif
  glViewport(0, 0, window.GetWidth(), window.GetHeight());

  glClearDepth(1.f);
  glClearColor(0.0f, 0.0f, .0f, 0.f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  //Enable lighting and set some color
  GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
  GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
  glEnable(GL_LIGHTING);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
}

/**
 * Initialize resources relavant to the game:
 *  - shaders
 *  - models
 *  - camera
 */
void init() {
  particleSystemShader = new Shader("shaders/psystem");
  if (!particleSystemShader->loaded()) {
    cerr << particleSystemShader->errors() << endl;
    exit(-1);
  }

  simpleShader = new Shader("shaders/simple");
  if (!simpleShader->loaded()) {
    cerr << "Error loading simple shader in main."<<endl;
    cerr << simpleShader->errors() <<endl;
    exit(-1);
  }
  avatar = new Avatar();
  avatar->setShader(particleSystemShader);

  camera = new Camera(nearClip, farClip, fov, initWinHeight, initWinWidth);

  mazeString = "ff";
  maze = new Maze(mazeString, particleSystemShader);
  creep = new Creep(simpleShader, maze);
  window.ShowMouseCursor(false);
}
/**
 * The following functions change how the maze looks based on user input.
 *
 * This should probably be its own class, and is probably the messiest
 * part of my code :(
 */
void tryAndAddTileUp() {
  //Can not add an upward tile if the last tile confirmed is down
  if (mazeString.at(mazeString.length() - 2) != 'd') {
    mazeString.erase(mazeString.length() - 1);
    mazeString += "u";
  }
}

void tryAndAddForward() {
  //can always go forward.
  mazeString.erase(mazeString.length() - 1);
  mazeString += "f";
}

void tryAndAddDown() {
  //Can not add a downward tile if the last tile confirmed is up
  if (mazeString.at(mazeString.length() - 2) != 'u') {
    mazeString.erase(mazeString.length() - 1);
    mazeString += "d";
  }
}

void tryAndAddLeft() {
  //Can only add left if last confirmed is forward or another left
  if (mazeString.at(mazeString.length() - 2) == 'f' || mazeString.at(
      mazeString.length() - 2) == 'l') {
    mazeString.erase(mazeString.length() - 1);
    mazeString += "l";
  }
}

void tryAndAddRight() {
  //Can only add right if last confirmed is forward
  if (mazeString.at(mazeString.length() - 2) == 'f' || mazeString.at(
      mazeString.length() - 2) == 'r') {
    mazeString.erase(mazeString.length() - 1);
    mazeString += "r";
  }
}

/**
 * Takes appropriate action when keys are pressed by delegating
 * to appropriate function
 */
void keyPressed(sf::Key::Code key) {
  if (key == sf::Key::A) {
    camera->moveLeft();
  } else if (key == sf::Key::D) {
    camera->moveRight();
  } else if (key == sf::Key::W) {
    camera->moveForward();
  } else if (key == sf::Key::S) {
    camera->moveBackwards();
  } else if (key == sf::Key::E) {
    //Move selected tile up
    maze->selectedInc();
  } else if (key == sf::Key::Q) {
    //Move selected tile down
    maze->selectedDec();
  } else if (key == sf::Key::Num1) {
    maze->addTurret(TurretFactory::FIRE_WHEEL);
  } else if (key == sf::Key::Num2) {
    maze->addTurret(TurretFactory::LIGHTNING);
  } else if (key == sf::Key::Num3) {
    maze->addTurret(TurretFactory::GRAVITY_RAIN);
  } else if (key == sf::Key::Up) {
    //if last orientation is forward, we try up
    if (mazeString.at(mazeString.length() - 1) == 'f') {
      tryAndAddTileUp();
      //else if last orientation was down we try forward
    } else if ((mazeString.at(mazeString.length() - 1) == 'd')) {
      tryAndAddForward();
    }
  } else if (key == sf::Key::Down) {
    //if last orientation is forward, we try down
    if (mazeString.at(mazeString.length() - 1) == 'f') {
      tryAndAddDown();
      //else if last orientation was up we try forward
    } else if ((mazeString.at(mazeString.length() - 1) == 'u')) {
      tryAndAddForward();
    }
  } else if (key == sf::Key::Left) {
    //if last orientation is forward, we try left
    if (mazeString.at(mazeString.length() - 1) == 'f') {
      tryAndAddLeft();
      //else if last orientation was right we try forward
    } else if ((mazeString.at(mazeString.length() - 1) == 'r')) {
      tryAndAddForward();
    }
  } else if (key == sf::Key::Right) {
    //if last orientation is forward, we try left
    if (mazeString.at(mazeString.length() - 1) == 'f') {
      tryAndAddRight();
      //else if last orientation was right we try forward
    } else if ((mazeString.at(mazeString.length() - 1) == 'l')) {
      tryAndAddForward();
    }
  } else if (key == sf::Key::Return) {
    //confirm selection and add a new forward to choose
    mazeString += "f";
  }
}

/**
 * Handles mouse movement by changing current angle
 * and updating where we are looking at.
 */
void mouseMoved(int mouseX, int mouseY) {
  float deltaX = mouseX - 0.5f * window.GetWidth();
  float deltaY = mouseY - 0.5f * window.GetHeight();
  camera->rotate(deltaX, deltaY);
  window.SetCursorPosition(0.5f * window.GetWidth(), 0.5f * window.GetHeight());
}

/**
 * Clears up memory
 */
void cleanup() {
  delete avatar;
  delete camera;
  delete particleSystemShader;
  delete maze;
}
/**
 * Checks the event queue and delegates appropriately
 */
void handleInput() {
  sf::Event evt;
  while (window.GetEvent(evt)) {
    switch (evt.Type) {
      case sf::Event::Closed:
        cleanup();
        window.Close();
        break;
      case sf::Event::KeyPressed:
        keyPressed(evt.Key.Code);
        break;
      case sf::Event::MouseMoved:
        if (window.GetInput().IsMouseButtonDown(sf::Mouse::Left)) {
          if (!mouseReady) {
            window.SetCursorPosition(0.5f * window.GetWidth(),
                0.5f * window.GetHeight());
            mouseReady = true;
          } else {
            mouseMoved(evt.MouseMove.X, evt.MouseMove.Y);
          }
        } else {
          mouseReady = false;
        }
        break;
    }
  }
}

void renderScene() {
  camera->posCameraSetupView();

  GLfloat lightPosition[] = { camera->posX(), camera->posY(), camera->posZ()
      + .5, 0.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  //Debugging triangle. This helps me not get lost in the scene :D
  glColor4f(.23, .25, .9, .1);
  glBegin(GL_TRIANGLES);
  glVertex3f(-.5, 0, 0);
  glVertex3f(.5, 0, 0);
  glVertex3f(0, .5, 0);
  glEnd();

  glPopMatrix();
  maze->render(window.GetFrameTime());
  creep->render(window.GetFrameTime());
  avatar->render(window.GetFrameTime());
}

int main() {
  glInit();
  init();
  while (window.IsOpened()) {
    //I prefer to keep input handling in its own class, but
    //I was a little rushed to just get stuff done and the input
    //was not too dynamic in this game.
    handleInput();
    //Set the avatar position to be in front of the camera.
    avatar->updatePosition(camera->posX() + camera->atX(),
        camera->posY() + camera->atY(), camera->posZ() + camera->atZ() + .05,
        camera->totalXAngle(), camera->totalYAngle(), camera->sideDirection());

    //update maze
    maze->mazeStringIs(mazeString);
    renderScene();
    window.Display();
  }
}
