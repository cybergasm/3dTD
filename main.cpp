#include <iostream>

#include <bullet/btBulletDynamicsCommon.h>

#include "Framework.h"
#include "Avatar.h"
#include "Camera.h"
#include "Shader.h"

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

//Where the camera initially is
float eye_x = 0.0f;
float eye_y = 0.0f;
float eye_z = -1.0f;

//Where we are initially lookinglooking
float at_x = 0.0f;
float at_y = 0.0f;
float at_z = 0.0f;

//We set this flag to make sure that moving the mouse around then trying
//to move does not cause jumpy motion
bool mouseReady = false;

//Projection configuration
float fov = 45;
float nearClip = .1f;
float farClip = 500.0f;

float aspect = initWinWidth / initWinWidth;

/**
 * Avatar configuration
 */
Avatar* avatar;

/**
 * Shaders
 */

//Shader for anything rendered as a particle system
Shader* particleSystemShader;

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

  avatar = new Avatar(eye_x + at_x, eye_y + at_y, eye_z - 1);
  avatar->setShader(particleSystemShader);

  camera = new Camera(nearClip, farClip, fov, initWinHeight, initWinWidth);
  window.ShowMouseCursor(false);
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


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glPushMatrix();
  //Debugging triangle. This helps me not get lost in the scene :D
  glColor4f(.23, .25, .9, .1);
  glBegin(GL_TRIANGLES);
  glVertex3f(-.5, 0, 0);
  glVertex3f(.5, 0, 0);
  glVertex3f(0, .5, 0);
  glEnd();

  glColor4f(.5, .6, .7, 1);
  glBegin(GL_QUADS);
  //top platform side
  glVertex3f(-.5, 0, -.5);
  glVertex3f(.5, 0, -.5);
  glVertex3f(.5, 0, .5);
  glVertex3f(-.5, 0, .5);
  glColor4f(.2, .2, .7, 1);
  //right side
  glVertex3f(.5, 0, -.5);
  glVertex3f(.5, -.1, -.5);
  glVertex3f(.5, -.1, .5);
  glVertex3f(.5, 0, .5);
  glColor4f(.5, .1, .1, 1);
  //left side
  glVertex3f(-.5, 0, -.5);
  glVertex3f(-.5, -.1, -.5);
  glVertex3f(-.5, -.1, .5);
  glVertex3f(-.5, 0, .5);
  glColor4f(.8, .6, .1, 1);
  //top side
  glVertex3f(-.5, 0, -.5);
  glVertex3f(-.5, -.1, -.5);
  glVertex3f(.5, -.1, -.5);
  glVertex3f(.5, 0, -.5);
  glColor4f(.1, .6, .1, 1);
  //top side
  glVertex3f(-.5, 0, .5);
  glVertex3f(-.5, -.1, .5);
  glVertex3f(.5, -.1, .5);
  glVertex3f(.5, 0, .5);
  glColor4f(1.0, 1.0, 1.0, 1);
  //bottom platform side
  glVertex3f(-.5, -.1, -.5);
  glVertex3f(.5, -.1, -.5);
  glVertex3f(.5, -.1, .5);
  glVertex3f(-.5, -.1, .5);
  glEnd();

  glTranslatef(0, 0, 1.1);

  glColor4f(.5, .6, .7, 1);
  glBegin(GL_QUADS);
  //top platform side
  glVertex3f(-.5, 0, -.5);
  glVertex3f(.5, 0, -.5);
  glVertex3f(.5, 0, .5);
  glVertex3f(-.5, 0, .5);
  glColor4f(.2, .2, .7, 1);
  //right side
  glVertex3f(.5, 0, -.5);
  glVertex3f(.5, -.1, -.5);
  glVertex3f(.5, -.1, .5);
  glVertex3f(.5, 0, .5);
  glColor4f(.5, .1, .1, 1);
  //left side
  glVertex3f(-.5, 0, -.5);
  glVertex3f(-.5, -.1, -.5);
  glVertex3f(-.5, -.1, .5);
  glVertex3f(-.5, 0, .5);
  glColor4f(.8, .6, .1, 1);
  //top side
  glVertex3f(-.5, 0, -.5);
  glVertex3f(-.5, -.1, -.5);
  glVertex3f(.5, -.1, -.5);
  glVertex3f(.5, 0, -.5);
  glColor4f(.1, .6, .1, 1);
  //top side
  glVertex3f(-.5, 0, .5);
  glVertex3f(-.5, -.1, .5);
  glVertex3f(.5, -.1, .5);
  glVertex3f(.5, 0, .5);
  glColor4f(1.0, 1.0, 1.0, 1);
  //bottom platform side
  glVertex3f(-.5, -.1, -.5);
  glVertex3f(.5, -.1, -.5);
  glVertex3f(.5, -.1, .5);
  glVertex3f(-.5, -.1, .5);
  glEnd();
  glPopMatrix();

  avatar->render(window.GetFrameTime());
}

int main() {
  glInit();
  init();
  while (window.IsOpened()) {
    handleInput();

    //Set the avatar position to be in front of the camera.
    avatar->updatePosition(camera->posX() + camera->atX(),
        camera->posY() + camera->atY(), camera->posZ() + camera->atZ() + .05,
        camera->totalXAngle(), camera->totalYAngle(), camera->sideDirection());

    renderScene();
    window.Display();
  }
}
