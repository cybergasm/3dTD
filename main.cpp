#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(800, 600), "CS248 Rules!", sf::Style::Close,
    settings);

void glInit() {
  //Viewing
  glViewport(0, 0, window.GetWidth(), window.GetHeight());

  // Set color and depth clear value
  glClearDepth(1.f);
  glClearColor(0.6f, 0.3f, .2f, 0.f);

  // Enable Z-buffer read and write
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  // Setup a perspective projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.f, 1.f, 1.f, 500.f);

  //  Set the camera
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0f, 0.0, 2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main() {
  glInit();
  while (window.IsOpened()) {
    // Event loop, for processing user input, etc.  For more info, see:
    // http://www.sfml-dev.org/tutorials/1.6/window-events.php
    sf::Event evt;
    while (window.GetEvent(evt)) {
      switch (evt.Type) {
        case sf::Event::Closed:
          // Close the window.  This will cause the game loop to exit,
          // because the IsOpened() function will no longer return true.
          window.Close();
          break;
      }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(.23, .25, .9);
    glBegin(GL_TRIANGLES);
    glVertex3f(-.1, 0, 0);
    glVertex3f(.1, 0, 0);
    glVertex3f(0, .1, 0);
    glEnd();
    window.Display();
  }
}
