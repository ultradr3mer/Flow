#ifndef _BASEINC_H_
    #define _BASEINC_H_

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <utility>
#include <iostream>
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
//#include <gl3.h>

#include <GL/glew.h> 
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

#include <noise\noise.h>
#include "noiseutils.h"

//using namespace noise;
using namespace glm;
using namespace std;

#include "ListContainer.h"
#include "GameBase.h"

const int MaxNameLen = 64;

const float pi = 3.1415926535897932384626433832795f;
const int screenX = 1280;
const int screenY = 720;
extern bool useVertexArrays;

//void updateError();
void updateError(char* location);

extern char execPath[512];
extern int execPathLenght;

//#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
//#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

#endif