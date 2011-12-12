#ifndef __CUDA_FUNCTION_H__
#define __CUDA_FUNCTION_H__

#include <iostream>
#include <vector>

#ifdef _WIN32
#include "GL/glew.h"
#else
#include <GL/glew.h>
#endif
#include "Object.hpp"

typedef struct _flake
{
	float x;
	float y;
	float z;
} Flake;

class SnowManager
{
	private :
		unsigned int m_uiNbFlakes;
		int m_iXMax;
		int m_iYMax;
		int m_iZMax;
		float m_Size;
		float m_Vent;

		Flake * m_vFlakes;
		GLfloat * cameraLastPos;

		GLuint textureId;

	public:
		SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax);
		void init();
		void moveFlakes();
		void update(const float * posCamera);
};

#endif
