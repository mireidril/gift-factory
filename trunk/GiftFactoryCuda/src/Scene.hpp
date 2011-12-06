#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "Utils.hpp"
#include "Object.hpp"
#include "Application.hpp"

class Object;
class Camera;

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
		Scene();
		~Scene();

		void init();
		void initFBODatas();
		void render();
		void loadObj(const std::string setFile);
		void update();
		void drawSquare();

		inline Camera* getCamera() {return m_camera;};

	private :
		FBODatas m_fboDatas;
		//Objets id
		std::vector<Object*> objects;
		Camera* m_camera;
};

#endif
