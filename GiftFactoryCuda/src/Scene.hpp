#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "Utils.hpp"
#include "Object.hpp"

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
		Scene(Application* parentApp);
		~Scene();

		void init();
		void initFBODatas();
		void render();
		void loadObj(const std::string setFile);
		void update();
		void drawSquare();

		inline Spline* getSpline () {return _spline;};

	private :
		FBODatas m_fboDatas;
		//Objets id
		std::vector<Object*> objects;
		Application* m_parentApp;
		Spline* _spline;
		
};

#endif
