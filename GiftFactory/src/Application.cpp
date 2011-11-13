#include "Application.hpp"
#include "Scene.hpp"
#include "ShaderManager.hpp"

Application::Application()
: m_bRunning(true)
, screen(NULL)
, m_uiWindowX(0)
, m_uiWindowY(0)
, m_uiWindowWidth(800)
, m_uiWindowHeight(600)
, m_iMousePositionX(-1)
, m_iMousePositionY(-1)
, m_bButtonPressed(false)
, m_bUseGeometryShader(true)
{
	m_vScenes.clear();
	m_vSceneRendered.clear();
}

Application::~Application()
{
	ShaderManager::destroy();

	m_vScenes.clear();
	m_vSceneRendered.clear();

	SDL_Quit();
}

void Application::init()
{
	// SDL initialisation
	initSDL();
	// Glew initialisation
	initExtensionsGlew();
	// OpenGL initialisation
	initGL();
	// Scene loadings
	initScenes();
}

void Application::initSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		std::cerr << "Probleme pour initialiser SDL : " << SDL_GetError() << std::endl;
		m_bRunning = false;
	}

	SDL_WM_SetCaption("Gift Factory", NULL);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	screen = SDL_SetVideoMode(m_uiWindowWidth, m_uiWindowHeight, 32, SDL_OPENGL | SDL_RESIZABLE);
	if(screen == NULL)
	{
		m_bRunning = false;
	}
}

void Application::initExtensionsGlew()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Error : " << glewGetErrorString(err) << std::endl;
		m_bRunning = false;
	}

	char* extensions_needed[6] = {"GL_ARB_shading_language_100", "GL_ARB_shader_objects", "GL_ARB_vertex_shader", "GL_ARB_fragment_shader", "GL_ARB_geometry_shader", "GL_ARB_multitexture"};
	for(unsigned int i = 0; i < 6; ++i)
	{
		if(!glewIsSupported(extensions_needed[i]))
		{
			std::cerr << extensions_needed[i] << " not supported" << std::endl;
			//if geometry shader are not supported, we don't quit the application
			if(i == 4)
			{
				m_bUseGeometryShader = false;
			}
			else
			{
				m_bRunning = false;
			}
		}
	}
}

void Application::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// glEnable & glDisable
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	reshapeWindow(m_uiWindowWidth, m_uiWindowHeight);
}

void Application::initScenes()
{
	// TODO : A REMPLACER
	m_vScenes.push_back(Scene());
	m_vScenes[0].init();
	m_vSceneRendered.push_back(0);
}

void Application::reshapeWindow(int iNewWidth, int iNewHeight)
{
	//TODO : EN OPENGL3 ?
	float ratio = (float)iNewWidth/(float)iNewHeight;
	glViewport(0, 0, (GLint)iNewWidth, (GLint)iNewWidth);

	m_uiWindowWidth = iNewWidth;
	m_uiWindowHeight = iNewWidth;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Application::checkEvents()
{
	// Event Processing
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				m_bRunning = false;
				break;
			case SDL_VIDEORESIZE:
				reshapeWindow(event.resize.w, event.resize.h);
				break;
			case SDL_MOUSEBUTTONDOWN: 
			case SDL_MOUSEBUTTONUP:
				mouseButtonEvent(&(event.button));
				break;
			case SDL_KEYDOWN:
				keyboardEvent(&(event.key));
				break;
			case SDL_MOUSEMOTION:
				mouseMotionEvent(&(event.motion));
				break;
			default:
				break;
		}
	}
}

void Application::keyboardEvent(SDL_KeyboardEvent *event)
{
	switch(event->keysym.sym)
	{
		// Exit
		case SDLK_ESCAPE:
		case SDLK_q:
			m_bRunning = false;
			break;
		// WireFrame
		case SDLK_w:
			GLint wtype[2];
			glGetIntegerv(GL_POLYGON_MODE, wtype);
			if(wtype[0] == GL_FILL)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
		default:
			break;
	}
}

void Application::mouseButtonEvent(SDL_MouseButtonEvent *event)
{
	if( event->button == int(SDL_BUTTON_LEFT) && event->state == int(SDL_PRESSED) )
	{
		m_iMousePositionX = event->x;
		m_iMousePositionY = event->y;
		m_bButtonPressed = true;
	}

	if( event->button == int(SDL_BUTTON_LEFT) && event->state == int(SDL_RELEASED) )
	{
		m_bButtonPressed = false;
	}
}

void Application::mouseMotionEvent(SDL_MouseMotionEvent *event)
{
	if(m_bButtonPressed)
	{
		//TODO : Rotation de la caméra pour les tests ?
	}
}

void Application::update()
{
	while(m_bRunning)
	{
		checkEvents();

		for(unsigned int i = 0; i < m_vSceneRendered.size(); ++i)
		{
			unsigned int uiId = m_vSceneRendered[i];
			m_vScenes[uiId].render();
		}
	}
}

bool Application::isRunning()
{
	return m_bRunning;
}