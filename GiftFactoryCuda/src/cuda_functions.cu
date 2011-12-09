#include "GL/glew.h"
#include "CUDA/cuda.h"
#include "GL/glu.h"

#include "cuda_functions.h"

#include "GL/glu.h"

#include "cuda_functions.h"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"
#include "TGALoader.h"


// Kernel that executes on the CUDA device
__global__ void cuda_render(Flake * lol)
{
	//int index = blockIdx.x * blockDim.x + threadIdx.x;
	lol[0].y -= 0.001;
}

SnowManager::SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax)
: m_uiNbFlakes(nbFlakes)
, m_iXMax(xMax)
, m_iYMax(yMax)
, m_iZMax(zMax)
, textureId(0)
{
	if( m_uiNbFlakes > 0)
	{
		m_vFlakes = new Flake[m_uiNbFlakes];
		for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
		{
			Flake f;
			//At the start, random position
			f.x = 0.f - (float)i/10;
			f.y = 0.f;
			f.z = -1.f;
			f.size = 12.f;

			m_vFlakes[i] = f;
		}
	}
}

void SnowManager::init()
{
	TGALoader oglt;
	oglt.LoadOpenGLTexture("textures/particle.tga", &textureId, TGA_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void SnowManager::update(const float* posCamera)
{
	/*for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		m_vFlakes[i].x = posCamera[0] + (float) i/10;
		m_vFlakes[i].y = posCamera[1] + 0;
		m_vFlakes[i].z = posCamera[2] + -1.f;
	}*/

	//Flakes position updating
	Flake *a_d;
	size_t size = m_uiNbFlakes * sizeof(Flake);
	cudaMalloc((void **) &a_d, size);
	cudaMemcpy(a_d, m_vFlakes, size, cudaMemcpyHostToDevice);
	cuda_render <<< m_uiNbFlakes, 1 >>> ((Flake *)a_d);
	cudaMemcpy(m_vFlakes, a_d, sizeof(Flake)*m_uiNbFlakes, cudaMemcpyDeviceToHost);
	cudaFree(a_d);

	glUseProgram(0);
	//glEnable(GL_POINT_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	glPointSize(12.f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexEnvf (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

	// Active le remplacement du point par la texture
	glEnable (GL_POINT_SPRITE_ARB);		

	glBegin(GL_POINTS);
	for(int i = 0; i < m_uiNbFlakes; ++i)
	{
		glVertex3f(m_vFlakes[i].x, m_vFlakes[i].y, m_vFlakes[i].z);
	}
	glEnd();

	// Désactive le remplacement du point par la texture
	glDisable (GL_POINT_SPRITE_ARB);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_POINT_SMOOTH);
}

void SnowManager::see()
{
	for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		std::cout << m_vFlakes[i].x<< std::endl;
	}
}