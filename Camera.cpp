/*
 * Camera.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: emint
 */
#include <math.h>
#include <GL/glu.h>

#include "assimp/aiTypes.h"

#include "Camera.h"

Camera::Camera(float nClip, float fClip, float fov_, int wH, int wW) :
  nearClip(nClip), farClip(fClip), fov(fov_), winHeight(wH), winWidth(wW),
      yAxisMax(.95), rateOfMovement(.1), totYAngle(0.0f), totXAngle(0.0f),
      sensitivity(1) {
  position.x = 0.1f;
  position.y = 0.1f;
  position.z = 1.0f;

  lookAt.x = 0.0f;
  lookAt.y = 0.0f;
  lookAt.z = 1.1f;
  lookAt.Normalize();

  upVec.x = 0.0f;
  upVec.y = 1.0f;
  upVec.z = 0.0f;
}

Camera::~Camera() {
}

/**
 * Simply returns the cross product to get the orthogonal vector
 */
aiVector3D Camera::sideDirection() {
  aiVector3D result;
  result.x = lookAt.y * upVec.z - upVec.y * lookAt.z;
  result.y = lookAt.z * upVec.x - upVec.z * lookAt.x;
  result.z = lookAt.x * upVec.y - upVec.x * lookAt.y;
  return result;
}

void Camera::wHeightIs(int height) {
  winHeight = height;
}

void Camera::wWidthIs(int width) {
  winWidth = width;
}

void Camera::rotate(int dX_, int dY_) {
  float dX = (float) dX_ / sensitivity;
  float dY = (float) dY_ / sensitivity;

  float degreesToRadians = M_PI / 180;

  //grab the angles looking up and down
  float upDownAngle = -2 * fov * degreesToRadians * dY / winHeight;
  float leftRightAngle = -2 * fov * degreesToRadians * dX / winWidth;

  //Accumulate a total rotation lr
  totXAngle += leftRightAngle;

  //Now we have to rotate up-down around whatever the side-axis is
  aiVector3D tempRot;
  aiVector3D side = sideDirection();
  aiMatrix4x4 rotateAroundSide = aiMatrix4x4::Rotation(upDownAngle, side,
      rotateAroundSide);

  //Attempt to rotate around side axis (e.g. up and down)
  aiVector3D newLook = rotateAroundSide * lookAt;

  //Can we not look farther up?
  if (fabs(newLook.y) < yAxisMax) {
    //if we can, make this the new view and add to the accumulation
    totYAngle += upDownAngle;
    lookAt = newLook;
  }

  //Look left and right
  aiMatrix4x4 rotateAroundY = aiMatrix4x4::RotationY(leftRightAngle,
      rotateAroundY);
  lookAt *= rotateAroundY;

  lookAt.Normalize();
}

void Camera::posCameraSetupView() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, winHeight / winWidth, nearClip, farClip);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(position.x, position.y, position.z, position.x + lookAt.x,
      position.y + lookAt.y, position.z + lookAt.z, upVec.x, upVec.y, upVec.z);
}
/**
 * Movement
 */
void Camera::moveBackwards() {
  position -= lookAt * rateOfMovement;
}

void Camera::moveForward() {
  position += lookAt * rateOfMovement;
}

void Camera::moveUp() {
  position += upVec * rateOfMovement;
}

void Camera::moveDown() {
  position -= upVec * rateOfMovement;
}

void Camera::moveLeft() {
  position -= sideDirection() * rateOfMovement;
}

void Camera::moveRight() {
  position += sideDirection() * rateOfMovement;
}

/**
 * Getters
 */

float Camera::posX() {
  return position.x;
}

float Camera::posY() {
  return position.y;
}

float Camera::posZ() {
  return position.z;
}

float Camera::atX() {
  return lookAt.x;
}

float Camera::atY() {
  return lookAt.y;
}

float Camera::atZ() {
  return lookAt.z;
}

float Camera::totalXAngle() {
  return totXAngle;
}

float Camera::totalYAngle() {
  return totYAngle;
}
