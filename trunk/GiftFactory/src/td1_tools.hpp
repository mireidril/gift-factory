#ifndef __TD1_TOOLS_HPP__
#define __TD1_TOOLS_HPP__

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

// Chargement de PPM
GLubyte * loadPPM(const char * const fn, unsigned int& w, unsigned int& h);

// Sphere version GL2-
void drawSphereDeprecated(const float& radius, const unsigned int& stepx, const unsigned int& stepy);

// Sphere GL3+ : construction VBOs
void buildSphereObjects(const float& radius, const unsigned int& stepx, const unsigned int& stepy, GLuint &verticesObjId, GLuint &texCoordsObjId, GLuint &normalsObjId, GLuint &facesObjId, int &nbVertices, int &nbFaces);

#endif
