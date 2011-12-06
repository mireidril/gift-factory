#ifndef __CUDA_FUNCTION_H__
#define __CUDA_FUNCTION_H__

#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "Object.hpp"

typedef struct _flake
{
	float x;
	float y;
	float z;
	float size;
} Flake;

class SnowManager
{
	private :
		unsigned int m_uiNbFlakes;
		float m_iXMax;
		float m_iYMax;
		float m_iZMax;
		Flake * m_vFlakes;

		GLuint m_uiShaderId;
		GLuint indiceId;
		GLuint textureId;
		GLuint vboId;

		GLfloat * m_flakesvertices;
		GLuint * m_flakesindices;

		Object * m_oFlakes;

	public:
		SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax);
		void init();
		void update(const float * posCamera);
		void see();
};
void launch_bb();

#endif
