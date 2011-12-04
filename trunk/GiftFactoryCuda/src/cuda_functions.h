#ifndef __CUDA_FUNCTION_H__
#define __CUDA_FUNCTION_H__

#include <iostream>
#include <vector>

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

		GLuint verticesObjId;
		GLuint textureId;

	public:
		SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax);
		void init();
		void update(const float * posCamera);
		void see();
};
void launch_bb();

#endif
