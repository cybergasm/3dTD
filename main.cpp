#include <iostream>

#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <bullet/btBulletDynamicsCommon.h>


#include "Avatar.h"

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
sf::Window window(sf::VideoMode(initWinHeight, initWinWidth), "CS248 Rules!",
    sf::Style::Close, settings);

/**
 * Camera and movement variables
 */
float eye_x = 0.1f;
float eye_y = 0.1f;
float eye_z = 1.0f;

//Where we are looking
float at_x = 0.0f;
float at_y = 0.0f;
float at_z = 1.1f;

//Specify the degree that the camera has turned due to mouse movement
//We will be using spherical coordinates
float yzDeg, xyDeg = 0.0f;

//The speed at which we move
const float movementSpeed = 8;

//Remember where mouse was last
int lastMouseX = initWinWidth / 2;
int lastMouseY = initWinHeight / 2;

/**
 * Creating a pointer to our avatar
 */
Avatar* avatar;

void glInit() {
  glViewport(0, 0, window.GetWidth(), window.GetHeight());

  glClearDepth(1.f);
  glClearColor(0.6f, 0.3f, .2f, 0.f);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
 * Moves the player's avatar
 */
void moveCameraAndPlayer(sf::Key::Code key) {
  if (key == sf::Key::A) {
    if (at_x > 0) {
      eye_x += -1 * at_x / movementSpeed;
      eye_z -= at_z / movementSpeed;
    } else {
      eye_x += at_x / movementSpeed;
      eye_z += at_z / movementSpeed;
    }
  } else if (key == sf::Key::D) {
    if (at_x > 0) {
      eye_x += at_x / movementSpeed;
      eye_z += at_z / movementSpeed;
    } else {
      eye_x += -1 * at_x / movementSpeed;
      eye_z -= at_z / movementSpeed;
    }
  } else if (key == sf::Key::W) {
    eye_x += at_x / movementSpeed;
    eye_y += at_y / movementSpeed;
    eye_z -= at_z / movementSpeed;
  } else if (key == sf::Key::S) {
    eye_x -= at_x / movementSpeed;
    eye_y -= at_y / movementSpeed;
    eye_z += at_z / movementSpeed;
  }
}
/**
 * Takes appropriate action when keys are pressed by delegating
 * to appropriate function
 */
void keyPressed(sf::Key::Code key) {
  if (key == sf::Key::A) {
    moveCameraAndPlayer(key);
  } else if (key == sf::Key::D) {
    moveCameraAndPlayer(key);
  } else if (key == sf::Key::W) {
    moveCameraAndPlayer(key);
  } else if (key == sf::Key::S) {
    moveCameraAndPlayer(key);
  }
}

/**
 * Handles mouse movement by changing current angle
 * and updating where we are looking at.
 */
void mouseMoved(int mouseX, int mouseY) {
  if (mouseX > lastMouseX && xyDeg <= M_PI / 4) {
    xyDeg += .01f;
  } else if (mouseX < lastMouseX && xyDeg > -M_PI / 4) {
    xyDeg -= .01f;
  }

  if (mouseY > lastMouseY && yzDeg >= -M_PI / 4) {
    yzDeg -= .01f;
  } else if (mouseY < lastMouseY && yzDeg <= M_PI / 4) {
    yzDeg += .01f;
  }

  lastMouseX = mouseX;
  lastMouseY = mouseY;

  at_x = 5 * cos(yzDeg) * sin(xyDeg);
  at_y = 5 * sin(yzDeg);// * sin(xyDeg);
  cout << "ats: " << at_x << " " << at_y << " " << at_z << endl;
}


/**
 * Clears up memory
 */
void cleanup(){
  delete avatar;
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
        cout << "eyes: " << eye_x << " " << eye_y << " " << eye_z << endl;
        break;
      case sf::Event::MouseMoved:
        if (window.GetInput().IsMouseButtonDown(sf::Mouse::Left)) {
          mouseMoved(evt.MouseMove.X, evt.MouseMove.Y);
          cout << "Degs:" << xyDeg << " " << yzDeg << endl;
        }
        break;
    }
  }
}

/**
 * Sets up our view by setting the projection parameters
 * and camera position/orientation
 */
void setupViewAndProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.f, 1.f, 1.f, 500.f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye_x, eye_y, eye_z, eye_x + at_x, eye_y + at_y, eye_z - at_z, 0.0,
      1.0f, 0.0f);
}

void renderScene() {
  setupViewAndProjection();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  avatar->render();

  glColor4f(.23, .25, .9, 1);
  glBegin(GL_TRIANGLES);
  glVertex3f(-.5, 0, -1);
  glVertex3f(.5, 0, -1);
  glVertex3f(0, .5, -1);
  glEnd();
}

void init () {
  avatar = new Avatar(eye_x + at_x, eye_y + at_y, eye_z - 1);
}
int main() {
  init();
  glInit();
  while (window.IsOpened()) {
    handleInput();
    avatar->updatePosition(eye_x + at_x, eye_y + at_y, eye_z - 1);
    renderScene();
    window.Display();
  }
}
