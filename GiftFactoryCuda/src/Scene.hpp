#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "Utils.hpp"
#include "Object.hpp"

#ifdef _WIN32
#include "irrKlang/irrKlang.h"
#else
#include <irrKlang.h>
#endif

class Object;
class Application;
class Camera;
class Spline;

class Scene
{
	public :
	struct FBODatas{
			unsigned int width, height;
			unsigned int fbo;
			GLenum * fboBuffs;
			GLenum * windowBuff;
			unsigned int colorTexture, depthTexture;
			unsigned int littleColorTexture, littleDepthTexture; 
	};
		Scene(Application* parentApp, std::string file_spline, std::string file);
		~Scene();

		void init();
		void initFBODatas();
		void render();
		void loadObj(const std::string setFile);
		void update();
		void drawSquare();

		inline Spline* getSpline () {return _spline;};
		inline Application* getParentApplication () {return m_parentApp;};

		//Light information
		GLfloat * m_lightPosition;
		GLfloat m_lightDiffuse[4];
		GLfloat m_lightAmbient[4];
		GLfloat m_lightSpecular[4];

	private :
		FBODatas m_fboDatas;
		//Objets id
		std::vector<Object*> objects;
		Application* m_parentApp;
		Spline* _spline;
		std::string m_file;

		//Sounds
		std::string m_MusicName;
		irrklang::ISoundEngine* m_soundEngine;
};

#endif
