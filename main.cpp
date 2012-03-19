#include <iostream>
#include <string.h>

#include <stdio.h>
#include <sstream>

#include "Framework.h"
#include "Avatar.h"
#include "Camera.h"
#include "Shader.h"
#include "Maze.h"
#include "TurretFactory.h"
#include "Creep.h"
#include "CreepManager.h"

#include <oglft/OGLFT.h>

using namespace std;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209
#endif

#define GL_CHECK(x) {\
  (x);\
  GLenum error = glGetError();\
  if (GL_NO_ERROR != error) {\
    printf("%s\n", gluErrorString(error));\
  }\
}

/**
 * Initial window parameters
 */
const int initWinHeight = 800;
const int initWinWidth = 600;

//Set-up the window settings and get a handle to a window
sf::WindowSettings settings(24, 8, 2);
sf::RenderWindow window(sf::VideoMode(initWinHeight, initWinWidth),
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
Shader* simpleShader;

/**
 * Turrets
 */
TurretFactory* turretFactory;

/**
 * Creeps
 */
CreepManager* creepManager;

/**
 * Logic state
 */
//if we are showing instructions
bool instructions = true;

/**
 * Instructions
 */
sf::Image instructionTexture;

/**
 * Score keeping
 */
int numCreepsEscaped = 0;
int numCreepsDead = 0;
//The font we use to write to screen
OGLFT::Monochrome* hudText;

/**
 * Currency values
 */
float soulPoints = 100.0f;
//timer for when we give points back
float soulPointIncrease = 0.0f;

/**
 * Portal
 */
sf::Image portalTexture;

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
  GLfloat lightDiffuse[] = { 0.34f, .64f, .86f, 1.0f };
  GLfloat lightSpecular[] = { 0.23f, 0.45f, 0.32f, 1.0f };
  GLfloat lightAmbient[] = { 0.4f, 0.3f, 0.1f, 1.0f };
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
    cerr << "error loading simple shader in main." << endl;
    cerr << simpleShader->errors() << endl;
    exit(-1);
  }

  avatar = new Avatar();
  avatar->setShader(particleSystemShader);

  camera = new Camera(nearClip, farClip, fov, initWinHeight, initWinWidth);

  mazeString = "ffff";
  turretFactory = new TurretFactory(particleSystemShader);
  maze = new Maze(mazeString, particleSystemShader, turretFactory);
  creepManager = new CreepManager(maze, turretFactory);
  window.ShowMouseCursor(false);

  instructionTexture.LoadFromFile("models/instructionscreen.jpg");
  portalTexture.LoadFromFile("models/creep_opacity_texture1.jpg");

  hudText = new OGLFT::Monochrome("fonts/KOMIKAGL.ttf", 24);

  if (hudText == NULL || !hudText->isValid()) {
    cerr << "Could not load font." << endl;
    exit(-1);
  }
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
  } else if (key == sf::Key::Num1 && soulPoints > 10) {
    maze->addTurret(TurretFactory::FIRE_WHEEL);
    soulPoints -= 10;
  } else if (key == sf::Key::Num2 && soulPoints > 20) {
    maze->addTurret(TurretFactory::LIGHTNING);
    soulPoints -= 20;
  } else if (key == sf::Key::Num3 && soulPoints > 20) {
    maze->addTurret(TurretFactory::GRAVITY_RAIN);
    soulPoints -= 20;
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
  } else if (key == sf::Key::Return && soulPoints > 5) {
    //confirm selection and add a new forward to choose
    mazeString += "f";
    soulPoints -= 5;
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
  delete creepManager;
  delete simpleShader;
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
        if (!instructions) {
          keyPressed(evt.Key.Code);
        }
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
      case sf::Event::MouseButtonPressed:
        if (window.GetInput().IsMouseButtonDown(sf::Mouse::Right)) {
          instructions = !instructions;
        }
        break;

    }
  }
}

void renderPortal() {
  aiVector3D vertices[4] = { aiVector3D(-.5, 1, 0), aiVector3D(.5, 1, 0),
      aiVector3D(.5, 0, 0), aiVector3D(-.5, 0, 0) };
  aiVector3D texCoords[4] = { aiVector3D(0, 0, 0), aiVector3D(1, 0, 0),
      aiVector3D(1, 1, 0), aiVector3D(0, 1, 0) };
  unsigned int vertexIndex[4] = { 0, 1, 2, 3 };

  GLint oldId;
  glGetIntegerv(GL_CURRENT_PROGRAM, &oldId);
  GL_CHECK(glUseProgram(simpleShader->programID()));

  GLint positionIn = glGetAttribLocation(simpleShader->programID(),
      "positionIn");
  GLint texId = glGetAttribLocation(simpleShader->programID(), "texCoordIn");
  GLint texture = glGetUniformLocation(simpleShader->programID(), "texture");
  GLint textureFlag = glGetUniformLocation(simpleShader->programID(),
      "texturing");

  if (positionIn == -1) {
    cerr << "Error getting position handle for instructions." << endl;
  }

  if (texId == -1) {
    cerr << "Error getting texture coords handle for instructions." << endl;
  }

  if (texture == -1) {
    cerr << "Error getting texture handle for instructions." << endl;
  }

  if (textureFlag == -1) {
    cerr << "Error getting texturing flag for instructions." << endl;
  }

  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glEnableVertexAttribArray(texId));

  glUniform1f(textureFlag, 1.0f);

  glUniform1i(texture, 0);
  glActiveTexture(GL_TEXTURE0);
  portalTexture.Bind();

  GL_CHECK(
      glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &vertices[0]));
  GL_CHECK(
      glVertexAttribPointer(texId, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &texCoords[0]));

  GL_CHECK(
      glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT,
          &vertexIndex[0]));

  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(texId));

  GL_CHECK(glUseProgram(oldId));
}

void renderScene() {
  camera->posCameraSetupView();

  GLfloat lightPosition[] = { camera->posX(), camera->posY(), camera->posZ()
      + .5, 0.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderPortal();
  maze->render(window.GetFrameTime());
  creepManager->renderCreeps(window.GetFrameTime());
  avatar->render(window.GetFrameTime());
}

/**
 * Full-screen quad containing the instructions graphic. Full-screen
 * quad was done with tips from:
 * http://www.opengl.org/archives/resources/faq/technical/transformations.htm
 */
void renderInstructions() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aiVector3D vertices[4] = { aiVector3D(-1, -1, -1), aiVector3D(1, -1, -1),
      aiVector3D(1, 1, 1), aiVector3D(-1, 1, -1) };
  aiVector3D texCoords[4] = { aiVector3D(0, 1, 0), aiVector3D(1, 1, 0),
      aiVector3D(1, 0, 0), aiVector3D(0, 0, 0) };
  unsigned int vertexIndex[4] = { 0, 1, 2, 3 };

  GLint oldId;
  glGetIntegerv(GL_CURRENT_PROGRAM, &oldId);
  GL_CHECK(glUseProgram(simpleShader->programID()));

  GLint positionIn = glGetAttribLocation(simpleShader->programID(),
      "positionIn");
  GLint texId = glGetAttribLocation(simpleShader->programID(), "texCoordIn");
  GLint texture = glGetUniformLocation(simpleShader->programID(), "texture");
  GLint textureFlag = glGetUniformLocation(simpleShader->programID(),
      "texturing");

  if (positionIn == -1) {
    cerr << "Error getting position handle for instructions." << endl;
  }

  if (texId == -1) {
    cerr << "Error getting texture coords handle for instructions." << endl;
  }

  if (texture == -1) {
    cerr << "Error getting texture handle for instructions." << endl;
  }

  if (textureFlag == -1) {
    cerr << "Error getting texturing flag for instructions." << endl;
  }

  GL_CHECK(glEnableVertexAttribArray(positionIn));
  GL_CHECK(glEnableVertexAttribArray(texId));

  glUniform1f(textureFlag, 1.0f);

  glUniform1i(texture, 0);
  glActiveTexture(GL_TEXTURE0);
  instructionTexture.Bind();

  GL_CHECK(
      glVertexAttribPointer(positionIn, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &vertices[0]));
  GL_CHECK(
      glVertexAttribPointer(texId, 3, GL_FLOAT, 0, sizeof(aiVector3D),
          &texCoords[0]));

  GL_CHECK(
      glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT,
          &vertexIndex[0]));

  GL_CHECK(glDisableVertexAttribArray(positionIn));
  GL_CHECK(glDisableVertexAttribArray(texId));

  GL_CHECK(glUseProgram(oldId));

}

/**
 * This function is, admittedly, a really big hack. I could
 * not get the color to change using default libraries so I switched
 * over to using my simple shader. But, as you might notice,
 * this also does not change it to the color you would expect, but
 * it does change it to white which is better than black on black.
 */
void updateScores(float framerate) {
  int newDead = creepManager->getNumDeadCreeps();
  numCreepsEscaped += creepManager->getNumEscapedCreeps();
  numCreepsDead += newDead;

  soulPoints += newDead * 10;
  soulPointIncrease += 10 * framerate;

  if (soulPointIncrease > 10.0f) {
    soulPoints += 1;
    soulPointIncrease = 0.0f;
  }

  //Give us a flat view to overlay the text
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLint oldId;
  glGetIntegerv(GL_CURRENT_PROGRAM, &oldId);

  //This is simply to make it use the default color which was black
  GL_CHECK(glUseProgram(simpleShader->programID()));

  //I was told to do this...
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //figure out our text
  stringstream formatter;
  formatter << "Evil killed: " << numCreepsDead << " Evil escaped: "
      << numCreepsEscaped << endl;
  hudText->draw(-1, -1, formatter.str().c_str());

  stringstream formatter2;
  formatter2 << "Soul points: " << soulPoints << endl;
  hudText->draw(-1, -.8, formatter2.str().c_str());
  //Get the hell out of here and pretend I never wrote such hacky crap...
  GL_CHECK(glUseProgram(oldId));

}

int main() {
  glInit();
  init();
  while (window.IsOpened()) {
    //I prefer to keep input handling in its own class, but
    //I was a little rushed to just get stuff done and the input
    //was not too dynamic in this game.
    handleInput();
    if (!instructions) {
      //Set the avatar position to be in front of the camera.
      avatar->updatePosition(camera->posX() + camera->atX(),
          camera->posY() + camera->atY(), camera->posZ() + camera->atZ() + .05,
          camera->totalXAngle(), camera->totalYAngle(), camera->sideDirection());

      //move the manager forward
      creepManager->updateTime(window.GetFrameTime());
      creepManager->updateCreeps();

      //update maze
      maze->mazeStringIs(mazeString);

      renderScene();

      updateScores(window.GetFrameTime());

      window.Display();
    } else {
      window.Display();
      renderInstructions();
    }
  }
}
