#pragma once
#ifndef _GLOBCONT_H_
    #define _GLOBCONT_H_

#include "CApp.h"
extern ListContainer<GameBase> GameObjList;
extern ListContainer<Model> SceneDrawables;
extern ListContainer<MaterialData> Materials;
extern ListContainer<ShaderData> Shaders;
extern ListContainer<MeshData> Meshes;
extern ListContainer<TextureData> Textures;
extern ListContainer<Sun> Lights;

#endif