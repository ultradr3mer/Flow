#pragma once
#include "Base.h"
extern char execPath[512];
extern int execPathLenght;
char* FullFileName(char* folder, char* file);
char* FileToBuf(char *file);
float max(float valA, float valB);
bool cutFromChar(char* sub, char seperator, char* whole, int* curIndex);
mat4 MatrixFromPosAng(vec3 Position, vec3 Rotation);
mat4 MatrixFromPosAngSiz(vec3 Position, vec3 Rotation, vec3 Size);
bool strcasecmp(char* str1,char* str2);
btVector3 BulletVec3FromVec3(vec3* origVector);
vec3 Vec3FromBulletVec3(btVector3* origVector);
vec3 RotationFromNormal(vec3 normal);
