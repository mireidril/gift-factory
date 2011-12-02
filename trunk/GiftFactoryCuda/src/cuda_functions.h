#ifndef __CUDA_FUNCTION_H__
#define __CUDA_FUNCTION_H__

#include <iostream>
#include <vector>

typedef struct _flake
{
	int x;
	int y;
	int size;
} Flake;

class SnowManager
{
	private :
		unsigned int m_uiNbFlakes;
		int m_iXMax;
		int m_iYMax;
		int m_iZMax;
		Flake * m_vFlakes;
	public:
		SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax);
		void update();
		void see();
};
void launch_bb();

#endif
