#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "Utils.hpp"
#include "model_obj.h"
#include "SDL/SDL_image.h"

class ShaderManager;

typedef std::map<std::string, GLuint> ModelTextures;

class Object
{
	public :
		Object(const char* filename, bool enableTextures = true, const char* shaderName = "", bool bMesh = true);
		~Object();

		void init();
		void sendVertices(GLfloat * vertices, const unsigned int & nbVertices);
		void draw(GLfloat* view);
		GLuint LoadTexture(const char *pszFilename);

		inline float * getTransformMat(){ return m_transformMat; };
		inline void setTransformMat( float * transformMat ){ m_transformMat = transformMat; };

	private :
		//false = Point Sprite, and true = Mesh
		bool m_bMesh;

		//Buffers
		GLuint m_uiVerticeBufferId;
		GLuint m_uiNbVertices;

		// Mesh Datas
		const char* objFileName;
		ModelOBJ	g_model;
		ModelTextures	g_modelTextures;

		//Textures
		bool g_enableTextures;
		unsigned int m_uiNbTextures;
		GLuint m_iTextureIds;
		std::string m_texFileName;

		//Shader
		const char* m_sShaderName;
		GLhandleARB m_uiShaderId;

		//ShaderManager
		ShaderManager* m_shaderManager;

		//Transformation matrix
		float * m_transformMat;
};

#endif
