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
void invertPicture( GLubyte * img, unsigned int& w, unsigned int& h );
void normalize (GLfloat * a);
GLfloat getNorm (GLfloat * a);
void vectorProduct (GLfloat * a, GLfloat * b, GLfloat * result);
void multMatrixBtoMatrixA(GLfloat * A, GLfloat * B);
void multMatrixBtoMatrixA(GLfloat * A, GLfloat * B, GLfloat * result);
void setToIdentity(GLfloat * matrix);
void setToTranslate(GLfloat * matrix, GLfloat * t);
void setToScale(GLfloat * matrix, GLfloat * s);
void setToRotate(GLfloat * matrix, GLfloat angle, GLfloat * axis);

#endif
