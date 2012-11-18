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

static void updateError();
#endif