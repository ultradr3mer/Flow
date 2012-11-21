#ifndef _BASEINC_H_
    #define _BASEINC_H_

#include <stdlib.h>
#include <stdio.h>
#include <utility>
#include <iostream>
using namespace std;
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
//#include <gl3.h>

#include <GL/glew.h> 
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

using namespace glm;

#include "GameBase.h"

const float pi = 3.1415926535897932384626433832795f;
const int screenX = 640;
const int screenY = 480;
extern bool useVertexArrays;

void updateError();

#endif