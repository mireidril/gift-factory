#include "../../Dependencies/include/CUDA/cuda.h"
//#include <CUDA/cuda.h>

#include "cuda_functions.h"

// Kernel that executes on the CUDA device
__global__ void cuda_render(Flake * lol)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	lol[0].x = 5;
}

SnowManager::SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax)
: m_uiNbFlakes(nbFlakes)
, m_iXMax(xMax)
, m_iYMax(yMax)
, m_iZMax(zMax)
{
	if( m_uiNbFlakes > 0)
	{
		m_vFlakes = new Flake[m_uiNbFlakes];
		for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
		{
			Flake f;
			//At the start, random position
			f.x = 0;
			f.y = 0;
			f.size = 0;
			m_vFlakes[i] = f;
		}
	}
}

void SnowManager::update()
{
	Flake *a_d;  // Pointer to host & device arrays
	size_t size = m_uiNbFlakes * sizeof(Flake);
	cudaMalloc((void **) &a_d, size);
	cudaMemcpy(a_d, m_vFlakes, size, cudaMemcpyHostToDevice);
	cuda_render <<< m_uiNbFlakes, 1 >>> ((Flake *)a_d);
	cudaMemcpy(m_vFlakes, a_d, sizeof(Flake)*m_uiNbFlakes, cudaMemcpyDeviceToHost);
}

void SnowManager::see()
{
	for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		std::cout << m_vFlakes[i].x<< std::endl;
	}
}

void launch_bb()
{
	std::cout<<"Coucou, chez moi j'ai CUDA !!!"<<std::endl;
}