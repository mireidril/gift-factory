#ifndef _USING_CUDA_

#include "cuda_functions.h"

#include "GL/glew.h"
#include "GL/glu.h"

#include "cuda_functions.h"
#include "TextureManager.hpp"

SnowManager::SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax)
: m_uiNbFlakes(nbFlakes)
, m_iXMax(xMax)
, m_iYMax(yMax)
, m_iZMax(zMax)
, verticesObjId(0)
, textureId(0)
{
	if( m_uiNbFlakes > 0)
	{
		m_vFlakes = new Flake[m_uiNbFlakes];
		for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
		{
			Flake f;
			//At the start, random position
			f.x = 0 - (float) i/10;
			f.y = 0;
			f.z = -1;
			f.size = 10;
			m_vFlakes[i] = f;
		}
	}
}

void SnowManager::init()
{
	// Init the texture data
	TextureManager::Texture *tex = new TextureManager::Texture();
	tex->texFileName = "textures/clouds.ppm";
	tex->shaderUniformName = "";
	// Load the texture image
	SDL_Surface *surf = IMG_Load(tex->texFileName.c_str());
	tex->texPicture = surf;
	
	if(tex->texPicture != NULL)
	{
		std::cout << "loaded : " << tex->texFileName << std::endl;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->texPicture->w, tex->texPicture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->texPicture->pixels);
	}
	else
	{
		std::cout << "failed to load : " << tex->texFileName << std::endl;
	}
}

void SnowManager::update(const float* posCamera)
{
	//Antialiasing sur les points
	glEnable (GL_POINT_SMOOTH);
	//Gestion de la transparence
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexEnvf (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable (GL_POINT_SPRITE);*/

	glPushMatrix();
	/*for(int i = 0; i < 	m_uiNbFlakes; ++i)
	{
		glPointSize (m_vFlakes[i].size);
		glColor4f(0.f, 0.f, 0.f, 1.0f);
		glBegin (GL_POINTS);
			glVertex3f(posCamera[0] + m_vFlakes[i].x, posCamera[1] + m_vFlakes[i].y, posCamera[2] + m_vFlakes[i].z);
		glEnd();
	}*/
	glPopMatrix();

	// Désactive le remplacement du point par la texture
    glDisable (GL_POINT_SPRITE);
	glDisable(GL_TEXTURE_2D);
}

void SnowManager::see()
{
	/*for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		std::cout << m_vFlakes[i].x<< std::endl;
	}*/
}

#endif
