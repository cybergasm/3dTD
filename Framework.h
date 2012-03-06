#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// GLEW must be included first, if we need it.
#define FRAMEWORK_USE_GLEW
#include <GL/glew.h>

// SFML automatically includes SDL headers
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Open Asset Import Library
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

#include <memory>
#include <iostream>

#endif
