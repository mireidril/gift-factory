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

	//Snow Manager
	m_SnowManager = new SnowManager(5000, 100, 100, 75);
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
	// TODO : A REMPLACER

	//tests spline 
	std::vector<Spline::PointSpline> verticesScene1;
	Spline::PointSpline v1;
	v1.position[0] = 0;
	v1.position[1] = 0;
	v1.position[2] = 12;
	v1.nbFrames = 1;
	v1.yaw = 0;
	v1.debutRotation = false;
	v1.finRotation = false;
	verticesScene1.push_back(v1);

	Spline::PointSpline v2;
	v2.position[0] = -2;
	v2.position[1] = 0;
	v2.position[2] = 9;
	v2.nbFrames = 1;
	v2.yaw = 0;
	v2.debutRotation = true;
	v2.finRotation = false;
	verticesScene1.push_back(v2);

	Spline::PointSpline v3;
	v3.position[0] = 0;
	v3.position[1] = 0;
	v3.position[2] = 6;
	v3.nbFrames = 1;
	v3.yaw = -90;
	v3.debutRotation = false;
	v3.finRotation = true;
	verticesScene1.push_back(v3);

	Spline::PointSpline v4;
	v4.position[0] = 2;
	v4.position[1] = -1;
	v4.position[2] = 5;
	v4.nbFrames = 1000;
	v4.yaw = 20;
	v4.selfRotate = true;
	v4.debutSelfRotate = 0;
	v4.finSelfRotate = 1000;
	v4.debutRotation = false;
	v4.finRotation = false;
	verticesScene1.push_back(v4);

	Spline::PointSpline v5;
	v5.position[0] = 3;
	v5.position[1] = 0;
	v5.position[2] = 2;
	v5.nbFrames = 1;
	v5.yaw = 20;
	v5.debutRotation = true;
	v5.finRotation = false;
	verticesScene1.push_back(v5);

	Spline::PointSpline v6;
	v6.position[0] = 1;
	v6.position[1] = 0;
	v6.position[2] = 1;
	v6.nbFrames = 1;
	v6.yaw = 45;
	v6.debutRotation = false;
	v6.finRotation = true;
	verticesScene1.push_back(v6);

	std::vector<Spline::PointSpline> verticesScene2;
	Spline::PointSpline v11;
	v11.position[0] = 0;
	v11.position[1] = 0;
	v11.position[2] = 12;
	v11.nbFrames = 1;
	v11.yaw = 0;
	v11.debutRotation = false;
	v11.finRotation = false;
	verticesScene2.push_back(v11);

	Spline::PointSpline v12;
	v12.position[0] = -2;
	v12.position[1] = 0;
	v12.position[2] = 9;
	v12.nbFrames = 1;
	v12.yaw = 0;
	v12.debutRotation = true;
	v12.finRotation = false;
	verticesScene2.push_back(v12);

	Spline::PointSpline v13;
	v13.position[0] = 0;
	v13.position[1] = 0;
	v13.position[2] = 6;
	v13.nbFrames = 1;
	v13.yaw = -90;
	v13.debutRotation = false;
	v13.finRotation = true;
	verticesScene2.push_back(v13);

	Spline::PointSpline v14;
	v14.position[0] = 2;
	v14.position[1] = -1;
	v14.position[2] = 5;
	v14.nbFrames = 1000;
	v14.yaw = 20;
	v14.selfRotate = true;
	v14.debutSelfRotate = 0;
	v14.finSelfRotate = 1000;
	v14.debutRotation = false;
	v14.finRotation = false;
	verticesScene2.push_back(v14);

	Spline::PointSpline v15;
	v15.position[0] = 3;
	v15.position[1] = 0;
	v15.position[2] = 2;
	v15.nbFrames = 1;
	v15.yaw = 20;
	v15.debutRotation = true;
	v15.finRotation = false;
	verticesScene2.push_back(v15);

	Spline::PointSpline v16;
	v16.position[0] = 1;
	v16.position[1] = 0;
	v16.position[2] = 1;
	v16.nbFrames = 1;
	v16.yaw = 45;
	v16.debutRotation = false;
	v16.finRotation = true;
	verticesScene2.push_back(v16);

	std::string path;
#ifdef _WIN32
	path = "";
#else
	path = "../";
#endif
	m_vScenes.push_back(new Scene(this, verticesScene1, path + "SET_test"));
	m_vScenes.push_back(new Scene(this, verticesScene2, path + "SET_test2"));
	std::cout<<"init scene 0"<<std::endl;
	m_vScenes[0]->init();
	std::cout<<"init scene 1"<<std::endl;
	m_vScenes[1]->init();
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
