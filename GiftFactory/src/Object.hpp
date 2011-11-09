#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "ShaderManager.hpp"

class Object
{
	public :
		Object();
		~Object();

		void init();
		void draw();

	private :
		//Textures
		unsigned int m_uiNbTextures;
		GLuint *m_iTextureIds;

		//Shader
		const char* m_sShaderName;
		GLhandleARB m_uiShaderId;

		//ShaderManager
		ShaderManager* m_shaderManager;

		//Transformation matrix
};

#endif
