#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "Utils.hpp"
#include "model_obj.h"
#include <SDL/SDL_image.h>

class ShaderManager;
class Scene;
class Spline;

typedef std::map<std::string, GLuint> ModelTextures;

class Object
{
	public :
		Object(Scene * scene, const char* filename, bool enableTextures = true);
		~Object();

		void init();
		void copy(Object * sameObj);
		void draw(GLfloat* view);
		GLuint LoadTexture(const char *pszFilename);

		inline float * getTransformMat(){ return m_transformMat; };
		inline void setTransformMat( float * transformMat ){ m_transformMat = transformMat; };

		void setSpline(Spline* spline);

		void move();

	private :
		//Scene
		Scene * m_parentScene;

		// Mesh Datas
		const char* objFileName;
		ModelOBJ *	g_model;
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

		//Counter
		unsigned int m_counter;
		int m_period;

		//Transformation matrix
		float * m_transformMat;

		//Light parameters
		GLfloat m_diffuse[4];
		GLfloat m_ambient[4];
		GLfloat m_specular[4];
		GLfloat m_shininess;

		Spline* m_spline;
		bool m_splineEnded;
};

#endif
