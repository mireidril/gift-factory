#ifndef _USING_CUDA_

#include "cuda_functions.h"

#include "GL/glu.h"

#include "cuda_functions.h"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"

SnowManager::SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax)
: m_uiNbFlakes(nbFlakes)
, m_iXMax(xMax)
, m_iYMax(yMax)
, m_iZMax(zMax)
, textureId(0)
{
	if( m_uiNbFlakes > 0)
	{
		size = 40.f;
		m_vFlakes = new Flake[m_uiNbFlakes];
		for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
		{
			Flake f;
			//At the start, random position
			f.x = rand()%100 - 50;
			f.y = rand()%100 - 50;
			f.z = - rand()%100;

			m_vFlakes[i] = f;
		}
	}
}

void SnowManager::init()
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	TextureManager::Texture * tex = NULL;
	tex = TextureManager::addAndLoadTexture("particle.png", "");
	
	if (tex->texPicture)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->texPicture->w, tex->texPicture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->texPicture->pixels);
	}
	else
		std::cout<<"Problem when loading 'particle.png'"<<std::endl;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	float fMaxSize = 64.0f;
	float coeffs[] = { 1.0f, .0f, 0.0001f };
    glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, coeffs);
    
    //Taille du point
    glPointParameterf (GL_POINT_SIZE_MAX, fMaxSize);
    glPointParameterf (GL_POINT_SIZE_MIN, 1.0f);
}

void SnowManager::update(const float* posCamera)
{
	for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		/*m_vFlakes[i].x = posCamera[0] + (float) i/10;
		m_vFlakes[i].y = posCamera[1];
		m_vFlakes[i].z = -1.f;*/
	}

	glUseProgram(0);
	//glEnable(GL_POINT_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	glPointSize(size);

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

void SnowManager::see()
{
	/*for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		std::cout << m_vFlakes[i].x<< std::endl;
	}*/
}

#endif
