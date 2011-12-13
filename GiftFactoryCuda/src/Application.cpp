#include "Application.hpp"
#include "Scene.hpp"
#include "ShaderManager.hpp"
#include "Camera.hpp"
#include "Spline.hpp"

#include "cuda_functions.h"

unsigned int Application::windowWidth = 800;
unsigned int Application::windowHeight = 600;

Application::Application()
: m_bRunning(true)
, screen(NULL)
, m_uiWindowX(0)
, m_uiWindowY(0)
, m_iMousePositionX(-1)
, m_iMousePositionY(-1)
, m_bButtonPressed(false)
, m_bUseGeometryShader(true)
, m_SnowManager(NULL)
{
	m_vScenes.clear();
}

Application::~Application()
{
	ShaderManager::destroy();

	m_vScenes.clear();

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
	// Shaders initialisation
	//initShaders();

	//Snow Manager : swow fall is the most realistic if xMax = yMax = zMax
	m_SnowManager = new SnowManager(3000, 30, 30, 30);
	m_SnowManager->init();

	// Scene loadings
	initScenes();

	//TODO : fonction setCamera
	m_camera =  new Camera(this);
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


	screen = SDL_SetVideoMode(Application::windowWidth, Application::windowHeight, 32, SDL_OPENGL | SDL_RESIZABLE);
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
	const GLubyte *GLVersionString = glGetString(GL_VERSION);
	std::cout<<"Version OpenGL "<<GLVersionString<<std::endl;

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

	reshapeWindow(Application::windowWidth, Application::windowHeight);
}

void Application::initScenes()
{
	m_vScenes.push_back(new Scene(this, "settings/spline2", "settings/SET_scene1_degeu"));

	std::string path;
#ifdef _WIN32
	path = "";
#else
	path = "../";
#endif

	//m_vScenes.push_back(new Scene(this, verticesScene1, path + "SET_test"));
	//m_vScenes.push_back(new Scene(this, verticesScene2, path + "SET_test2"));

	std::cout<<"init scene 0"<<std::endl;
	m_vScenes[0]->init();
	//std::cout<<"init scene 1"<<std::endl;
	//m_vScenes[1]->init();
	m_vSceneRendered = 0;
}

void Application::initShaders()
{
	//ShaderManager::getInstance()->addShaders("color", false);
	//ShaderManager::getInstance()->addShaders("texture2D", false);

	unsigned int i, j;
	std::stringstream buffer; std::string line;
	std::fstream stream("listShaders.txt", std::ios::in);
	
	while(std::getline(stream, line)){

		buffer.clear(); buffer.str(line);

		std::string shaderName; 
		buffer >> shaderName; 
		std::cout << "shader init : " << shaderName << std::endl;
		ShaderManager::getInstance()->addShaders(shaderName.c_str(), false);
	}
	
}
void Application::reshapeWindow(int iNewWidth, int iNewHeight)
{
	//TODO : EN OPENGL3 ?
	float ratio = (float)iNewWidth/(float)iNewHeight;
//	glViewport(0, 0, (GLint)iNewWidth, (GLint)iNewWidth);

	Application::windowWidth = iNewWidth;
	Application::windowHeight = iNewHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Copier de Arrows : 
	//SDL_VideoMode update (may be unnecessary, and not woking proberly on some operating systems) (Comment on MacOSX 10.6)
	screen = SDL_SetVideoMode(Application::windowWidth, Application::windowHeight, 0,  SDL_OPENGL | SDL_RESIZABLE);

    // Viewport transformation update to fit window size
    glViewport(0, 0, Application::windowWidth, Application::windowHeight);
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
	GLfloat * position;
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
		case SDLK_UP:
			position = getCamera()->getPosition();
			position[2] -= 1;
			getCamera()->setPosition(position);
			break;
		case SDLK_DOWN:
			position = getCamera()->getPosition();
			position[2] += 1;
			getCamera()->setPosition(position);
			break;
		case SDLK_LEFT:
			position = getCamera()->getPosition();
			position[0] -= 1;
			getCamera()->setPosition(position);
			break;
		case SDLK_RIGHT:
			position = getCamera()->getPosition();
			position[0] += 1;
			getCamera()->setPosition(position);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		checkEvents();
		//if the spline is at its end
		if (!m_camera->moveForward() && m_vSceneRendered<m_vScenes.size()-1){
			m_vSceneRendered++;
		}

		//For tests
		//getCamera()->updateView();


		unsigned int uiId = m_vSceneRendered;
		m_vScenes[uiId]->update();
		m_vScenes[uiId]->render();
		//Snow Manager
		m_SnowManager->update(m_camera->getAim());
		
		SDL_GL_SwapBuffers();
	}
}

bool Application::isRunning()
{
	return m_bRunning;
}

Scene* Application::getRenderedScene(){
	return m_vScenes[m_vSceneRendered];
}
