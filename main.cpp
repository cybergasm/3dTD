#include <iostream>

#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <bullet/btBulletDynamicsCommon.h>

using namespace std;
/**
 * Camera and movement variables
 */
float eye_x = 0.0f;
float eye_y = 0.0f;
float eye_z = 1.0f;

//Set-up the window settings and get a handle to a window
sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(800, 600), "CS248 Rules!", sf::Style::Close,
    settings);

void glInit() {
  glViewport(0, 0, window.GetWidth(), window.GetHeight());

  glClearDepth(1.f);
  glClearColor(0.6f, 0.3f, .2f, 0.f);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void keyPressed(sf::Key::Code key) {
  if (key == sf::Key::A) {
    eye_x -= .1f;
  } else if (key == sf::Key::D) {
    eye_x += .1f;
  } else if (key == sf::Key::W) {
    eye_z -= .6f;
  } else if (key == sf::Key::S) {
    eye_z += .6f;
  }
}

void handleInput() {
  sf::Event evt;
  while (window.GetEvent(evt)) {
    switch (evt.Type) {
      case sf::Event::Closed:
        window.Close();
        break;
      case sf::Event::KeyPressed:
        keyPressed(evt.Key.Code);
        cout<<eye_x<<" "<<eye_y<<" "<<eye_z<<endl;
        break;
    }
  }
}

void setupViewAndProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.f, 1.f, 1.f, 500.f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye_x, eye_y, eye_z, eye_x, eye_y, eye_z - 2, 0.0, 1.0, 0.0);
}

void renderPlayerAvatar() {
  glColor4f (.1, .6, .4, 1);

  glBegin(GL_QUADS);
  glVertex3f(eye_x - .1, eye_y - .1, eye_z - 2);
  glVertex3f(eye_x - .1, eye_y + .1, eye_z - 2);
  glVertex3f(eye_x + .1, eye_y + .1, eye_z - 2);
  glVertex3f(eye_x + .1, eye_y - .1, eye_z - 2);
  glEnd();
}
void renderScene() {
  setupViewAndProjection();


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderPlayerAvatar();

  glColor4f(.23, .25, .9, 1);
  glBegin(GL_TRIANGLES);
  glVertex3f(-.5, 0, -1);
  glVertex3f(.5, 0, -1);
  glVertex3f(0, .5, -1);
  glEnd();
}

int main() {
  glInit();
  while (window.IsOpened()) {
    handleInput();
    renderScene();
    window.Display();
  }
}
