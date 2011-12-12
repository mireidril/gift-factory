#include "GL/glew.h"
#include "GL/glu.h"

#ifdef _WIN32
#include "CUDA/cuda.h"
#else
#include <cuda.h>
#endif

#include "cuda_functions.h"
#include "TextureManager.hpp"


// Kernel that executes on the CUDA device
__global__ void cuda_render(Flake * vFlakes, unsigned int iYMax, unsigned int izMax)
{
	int index = blockIdx.x * blockDim.x + threadIdx.x;

	if(vFlakes[index].y < - (float)iYMax/2.f)
	{
		vFlakes[index].y += iYMax;
	}
	vFlakes[index].y -= (float) izMax/1000.f;
}

SnowManager::SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax)
: m_uiNbFlakes(nbFlakes)
, m_iXMax(xMax)
, m_iYMax(yMax)
, m_iZMax(zMax)
, m_Size (17.f)
, m_Vent(0)
, cameraLastPos(NULL)
, textureId(0)
{
	if( m_uiNbFlakes > 0)
	{
		m_vFlakes = new Flake[m_uiNbFlakes];
		for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
		{
			Flake f;
			//At the start, random position
			f.x = rand()%m_iXMax - (float)m_iXMax/2.f + (float) (rand()%100/100.f);
			f.y = rand()%m_iYMax - (float)m_iYMax/2.f + (float)m_iYMax/3.f + (float) (rand()%100/100.f);
			f.z = - rand()%m_iZMax + 1.f + (float) (rand()%100/100.f);

			m_vFlakes[i] = f;
		}
	}
}

void SnowManager::init()
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	TextureManager::Texture * tex = NULL;
	tex = TextureManager::addAndLoadTexture("bouboule.png", "");
	
	if (tex->texPicture)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->texPicture->w, tex->texPicture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->texPicture->pixels);
	}
	else
		std::cout<<"Problem when loading 'bouboule.png'"<<std::endl;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	float fMaxSize = 40.0f;
	float coeffs[] = { 1.0f, .0f, 0.0001f };
    glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, coeffs);
    
    //Taille du point
    glPointParameterf (GL_POINT_SIZE_MAX, fMaxSize);
    glPointParameterf (GL_POINT_SIZE_MIN, 1.0f);
}

void SnowManager::moveFlakes()
{

}

void SnowManager::update(const float* posCamera)
{
	if(cameraLastPos == NULL)
	{
		cameraLastPos = new GLfloat[3];
		cameraLastPos[0] = 0.f;
		cameraLastPos[1] = 0.f;
		cameraLastPos[2] = 0.f;
	}

	/*for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		//m_vFlakes[i].x += posCamera[0] - cameraLastPos[0];
		//m_vFlakes[i].y -= posCamera[1] - cameraLastPos[1];
		//m_vFlakes[i].z += posCamera[2] - cameraLastPos[2];

		m_vFlakes[i].x -= posCamera[0] - cameraLastPos[0];
		m_vFlakes[i].y += posCamera[1] - cameraLastPos[1];
		//m_vFlakes[i].z -= posCamera[2] - cameraLastPos[2];
	}*/

	for(unsigned int i = 0; i < 3; ++i)
	{
		cameraLastPos[i] = posCamera[i];
	}

	//Flakes position updating
	int block_size = 4;
	int n_blocks = m_uiNbFlakes/block_size + (m_uiNbFlakes%block_size == 0 ? 0:1);

	Flake *a_d;
	size_t size = m_uiNbFlakes * sizeof(Flake);
	cudaMalloc((void **) &a_d, size);
	cudaMemcpy(a_d, m_vFlakes, size, cudaMemcpyHostToDevice);
	cuda_render <<< n_blocks, block_size >>> ((Flake *)a_d, m_iYMax, m_iZMax);
	cudaMemcpy(m_vFlakes, a_d, sizeof(Flake)*m_uiNbFlakes, cudaMemcpyDeviceToHost);
	cudaFree(a_d);

	glUseProgram(0);
	//glEnable(GL_POINT_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	glPointSize(m_Size);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexEnvf (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

	// Active le remplacement du point par la texture
	glEnable (GL_POINT_SPRITE_ARB);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	for(int i = 0; i < m_uiNbFlakes; ++i)
	{
		glVertex3f(m_vFlakes[i].x, m_vFlakes[i].y, m_vFlakes[i].z);
	}
	glEnd();

	// Désactive le remplacement du point par la texture
	glDisable (GL_POINT_SPRITE_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glDisable(GL_POINT_SMOOTH);
}