#include "Object.hpp"

Object::Object()
: m_uiNbTextures(0)
, m_iTextureIds(NULL)
, m_sShaderName("")
{
	m_shaderManager = ShaderManager::getInstance();
}

Object::~Object()
{
	//Textures deletion ?
}

void Object::init()
{
	//Shader initialization
	m_uiShaderId = m_shaderManager->getShaderProgramId(m_sShaderName);
	if(m_uiShaderId == ERROR_VALUE)
	{
		//false if there are only vertex and fragment shader, or true if there are vertex, fragment and geometry shader
		m_uiShaderId = m_shaderManager->addShaders(m_sShaderName, false);
	}

	//Textures initialization ?

	//Buffer
}

void Object::draw()
{
	//Draw here
}