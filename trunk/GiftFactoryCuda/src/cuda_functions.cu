#include "GL/glew.h"
#include "CUDA/cuda.h"
#include "GL/glu.h"

#include "cuda_functions.h"

/*#include "GL/glu.h"

#include "TextureManager.hpp"
#include "ShaderManager.hpp"

// Kernel that executes on the CUDA device
__global__ void cuda_render(Flake * lol)
{
	//int index = blockIdx.x * blockDim.x + threadIdx.x;
	//lol[0].y -= 0.001;
}

SnowManager::SnowManager(const unsigned int & nbFlakes, const int & xMax, const int & yMax, const int & zMax)
: m_uiNbFlakes(nbFlakes)
, m_iXMax(xMax)
, m_iYMax(yMax)
, m_iZMax(zMax)
, m_uiShaderId(0)
, textureId(0)
, m_oFlakes(NULL)
{
	if( m_uiNbFlakes > 0)
	{
		m_vFlakes = new Flake[m_uiNbFlakes];
		m_flakesvertices = new GLfloat[m_uiNbFlakes* 4];
		m_flakesindices = new GLuint[m_uiNbFlakes];
		for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
		{
			Flake f;
			//At the start, random position
			f.x = 0.f - (float)i/10;
			f.y = 0.f;
			f.z = -1.f;

			m_flakesvertices[i*4 + 0] = f.x;
			m_flakesvertices[i*4 + 1] = f.y;
			m_flakesvertices[i*4 + 2] = f.z;
			m_flakesvertices[i*4 + 3] = 1.f;
			std::cout<<"coord : ("<<m_flakesvertices[i*4 + 0]<<","<<m_flakesvertices[i*4 + 1]<<","<<m_flakesvertices[i*4 + 2]<<")"<<std::endl;
			m_flakesindices[i] = i;

			f.size = 10;
			m_vFlakes[i] = f;
		}
	}
}

void SnowManager::init()
{
	// Creates a VBO id for a VBO to store the vertices
    //glGenBuffers(1, &vboId);
	//glGenBuffers(1, &indiceId);
    //glGenBuffers(1, &(this->colorsVboId));
    
    //glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
	//glBufferData(GL_ARRAY_BUFFER, m_uiNbFlakes*4*sizeof(GLfloat), m_flakesvertices, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indiceId);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiNbFlakes*sizeof(GLuint), m_flakesindices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_oFlakes = new Object("", true, "star", false);
	
	//-------

	//Shader initialization
	char * m_sShaderName = "star";
	ShaderManager * shaderM = ShaderManager::getInstance();
	m_uiShaderId = shaderM->getShaderProgramId(m_sShaderName);
	if(m_uiShaderId == ERROR_VALUE)
	{
		//false if there are only vertex and fragment shader, or true if there are vertex, fragment and geometry shader
		m_uiShaderId = shaderM->addShaders(m_sShaderName, false);
		std::cout<<"new shader"<<std::endl;
	}

	//-----

	
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
	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POINT_SPRITE);
	glPointSize (32.f);
	//Gestion de la transparence
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexEnvf (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    

	glPushMatrix();
	for(int i = 0; i < 	m_uiNbFlakes; ++i)
	{
		glColor4f(0.f, 0.f, 0.f, 1.0f);
		glBegin (GL_POINTS);
			glVertex3f(posCamera[0] + m_vFlakes[i].x, posCamera[1] + m_vFlakes[i].y, posCamera[2] + m_vFlakes[i].z);
		glEnd();
	}
	glPopMatrix();

	for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		m_flakesvertices[i*4 + 0] = 5;
		m_flakesvertices[i*4 + 1] = 0;
		m_flakesvertices[i*4 + 2] = 0;
		m_flakesvertices[i*4 + 3] = 1.f;
	}
	m_oFlakes->sendVertices(m_flakesvertices, m_uiNbFlakes);
	m_oFlakes->draw(NULL);

	//glUseProgram(m_uiShaderId);
	//glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//glBufferData(GL_ARRAY_BUFFER, m_uiNbFlakes*4*sizeof(GLfloat), m_flakesvertices, GL_DYNAMIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
	//glDrawArrays(GL_POINTS, 0, m_uiNbFlakes);
	//glDrawElements(GL_POINTS, m_uiNbFlakes, GL_UNSIGNED_INT, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glUseProgram(0);
	// Désactive le remplacement du point par la texture
    glDisable (GL_POINT_SPRITE);
	//glDisable(GL_TEXTURE_2D);
}

void SnowManager::see()
{
	for(unsigned int i = 0; i < m_uiNbFlakes; ++i)
	{
		std::cout << m_vFlakes[i].x<< std::endl;
	}
}*/