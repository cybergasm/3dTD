/*
 * Avatar.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: emint
 */

#include "Avatar.h"
#include <GL/glew.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <math.h>

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

Avatar::Avatar() :
  x(0), y(0), z(0), xAng(0.0f), yAng(0.0f), halfSideLen(.05),
      numParticles(1000), aniDuration(20), blurTimer(0), blurBuffer(0), usedBuffers(0),
      particles(numParticles, aniDuration) {
  sideVector.x = 1.0f;
  sideVector.y = 0.0f;
  sideVector.z = 0.0f;

  srand(time(NULL));

  initializeParticles();

  particles.modeIs(GL_LINE_LOOP);
}

Avatar::~Avatar() {
}

void Avatar::initializeParticles() {
  aiVector3D vel;
  uint lifespan;
  aiVector3D color;
  float flip;
  for (unsigned int particle = 0; particle < numParticles; particle++) {
    vel.x = (rand() % 10 + 1) / 1000.0f;
    flip = (rand() % 10 + 1) / 10.0f;
    if (flip > .5) {
      vel.x *= -1;
    }
    vel.y = (rand() % 10 + 1) / 1000.0f;
    flip = (rand() % 10 + 1) / 10.0f;
    if (flip > .5) {
      vel.y *= -1;
    }
    vel.z = (rand() % 10 + 1) / 1000.0f;
    flip = -(rand() % 10 + 1) / 10.0f;
    if (flip > .5) {
      vel.z *= 1;
    }
    vel.Normalize();
    vel /= 80.0f;

    cout << vel.x << " " << vel.y << " " << vel.z << " " << lifespan << endl;
    lifespan = (rand() % aniDuration + aniDuration * .1f);
    color = getParticleColor();
    cout << color.x << " " << color.y << " " << color.z << endl;
    particles.addParticle(vel, aiVector3D(-vel.x/lifespan, -vel.y/lifespan, 0.0), color, lifespan);
  }
  //Create a vector holding the position of the avatar
  //and set that to the origin of the particles
  aiVector3D curPosition(0, 0, 0);
  particles.uniformLocationIs(curPosition);
}

aiVector3D Avatar::getParticleColor() {
  int color = rand() % 4;
  aiVector3D colorVals;
  switch (color) {
    case 0:
      colorVals.x = 204.f / 255.f;
      colorVals.y = 234.f / 255.f;
      colorVals.z = 252.f / 255.f;
      break;
    case 1:
      colorVals.x = 61.f / 255.f;
      colorVals.y = 91.f / 255.f;
      colorVals.z = 110.f / 255.f;
      break;
    case 2:
      colorVals.x = 213.f / 255.f;
      colorVals.y = 227.f / 255.f;
      colorVals.z = 235.f / 255.f;
      break;
    case 3:
      colorVals.x = 107.f / 255.f;
      colorVals.y = 159.f / 255.f;
      colorVals.z = 191.f / 255.f;
      break;
  }
  return colorVals;
}

void Avatar::render(float framerate) {
  //This transformation ensures the orientation is keeping up
  //with our camera
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(xAng * (180.0f / M_PI), 0, 1, 0);
  glTranslatef(-x, -y, -z);

  glTranslatef(x,y,z);

  //Get the buffers our caller was using
  GLint currentDraw, currentRead;
  glGetIntegerv(GL_DRAW_BUFFER, &currentDraw);
  glGetIntegerv(GL_READ_BUFFER, &currentRead);

  //Grab the current scene into our accumulation buffer
  glAccum(GL_LOAD, 1);

  //Disable current buffers
  glDrawBuffer(currentDraw);
  glReadBuffer(currentRead);

  //enable the current auxilary buffer
  glDrawBuffer(GL_AUX0 + blurBuffer);

  //Here we make a check to see if it is time to clear this buffer
  //Basically every multiple of the buffer time passed we clear the buffer
  if (blurTimer % (4*blurBuffer+1) == 0) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  //gl
  particles.renderWithFramerate(framerate);

  if (usedBuffers < 4){
    usedBuffers++;
  }

  //switch back to no shader program
  GL_CHECK(glUseProgram(0));
  //undo our transformation
  glPopMatrix();

  //Go through our previous buffers and add each of them to the accumulation
  for (unsigned int curBuffer = 0; curBuffer < usedBuffers; curBuffer++) {
    int bufferToUse = curBuffer;
    //This makes sure we add the scenes in order of rendering; We want to start
    //one past the current and loop back to the current
    if (usedBuffers == 4) {
      bufferToUse = ((blurBuffer + 4)%4 + curBuffer)%4;
    }
    //enable this frame to add to accumulation buffer
    glReadBuffer (GL_AUX0 + bufferToUse);
    glAccum(GL_ACCUM, .15);
    //disable frame
    glReadBuffer (GL_AUX0 + bufferToUse);
  }

  //Now renable previous read buffer as we do not have to read from
  //aux anymore.
  glReadBuffer(currentRead);

  //Now disable the aux buffer and re-enable the previous buffer
  //so accumulation buffer can be draw into previous
  glDrawBuffer(GL_AUX0 + blurBuffer);
  glDrawBuffer(currentDraw);

  //Dump the accumulation buffer into our
  glAccum(GL_RETURN, 1);

  //Update the timer. I do a check just to make sure it is constrained a bit and reset
  //every once in a while
  if (blurTimer >= 10000) {
    blurTimer = 0;
  }

  //Take us forward in time and set the following buffer to be used
  //for the next scene
  blurTimer += 1;
  blurBuffer = (blurBuffer + 1) % 4;
}

void Avatar::updatePosition(float x_, float y_, float z_, float xAng_,
    float yAng_, aiVector3D sideVec) {
  x = x_;
  y = y_;
  z = z_;
  xAng = xAng_;
  yAng = yAng_;
  sideVector = sideVec;
}

void Avatar::setShader(Shader* shader_) {
  shader = shader_;
  particles.shaderIs(shader);
}
