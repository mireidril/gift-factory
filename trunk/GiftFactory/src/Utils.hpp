#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "SDL/SDL.h"
#include "GL/glew.h"
//#include "GL/glu.h"
#include "GL/glut.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <vector>
//#include <assert.h>

// PPM Loading
GLubyte * loadPPM(const char * const fn, unsigned int& w, unsigned int& h);

#endif
