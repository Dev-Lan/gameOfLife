#pragma once
#include <G3D/G3DAll.h>

// these are set by my compiler, but if you don't set that up, at least one of these lines should be uncommented
// #define DEBUG_ENABLED=true
// #define DEBUG_ENABLED=false

#if (DEBUG_ENABLED)
	const G3D::String DATA_DIR = "./";
#else
	const G3D::String DATA_DIR = "../Resources/"; // this should work for deploying on mac
#endif

/// textures
const G3D::String TEXTURE_DIR = DATA_DIR + "textures/";
const G3D::String DEFAULT_SEED = TEXTURE_DIR + "mainScreen.png";
const G3D::String MORE_INFO_SCREEN = TEXTURE_DIR + "moreInfo.png";


/// shaders
const G3D::String SHADER_DIR  = DATA_DIR + "shaders/";
const G3D::String GOL_SHADER = SHADER_DIR + "gol.*";



