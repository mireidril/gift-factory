#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "Utils.hpp"

class Scene;

//Main class of the program
class Application
{
	public :
		
		Application();
		virtual ~Application();

		void init();
		void initSDL();
		void initExtensionsGlew();
		void initGL();
		void initScenes();
		void initShaders();

		void reshapeWindow(int iNewWidth, int iNewHeight);

		void checkEvents();
		void keyboardEvent(SDL_KeyboardEvent *event);
		void mouseButtonEvent(SDL_MouseButtonEvent *event);
		void mouseMotionEvent(SDL_MouseMotionEvent *event);

		void update();

		bool isRunning();
		bool useGeometryShader();

		
		static unsigned int windowWidth;
		static unsigned int windowHeight;

	private :
		bool m_bRunning;
		
		//Window
		SDL_Surface *screen;
		const unsigned int m_uiWindowX;
		const unsigned int m_uiWindowY;

		//Mouse
		int m_iMousePositionX;
		int m_iMousePositionY;
		bool m_bButtonPressed;

		bool m_bUseGeometryShader;

		//Scenes
		std::vector<Scene*> m_vScenes;
		std::vector<unsigned int> m_vSceneRendered;		//Contains the position (of the vector "m_vScenes") of the scenes which are going to be rendered

};

#endif
