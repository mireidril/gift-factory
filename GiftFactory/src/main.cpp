#include <iostream>

#include "td1_tools.hpp"
#include "shaders.hpp"

// Origine de la fenetre
static const unsigned int windowx = 0;
static const unsigned int windowy = 0;

// Dimensions de la fenetre
static unsigned int windowwidth  = 800;
static unsigned int windowheight = 600;

// Gestion des textures
static const unsigned int nbtextures = 3;
GLuint texturesid[nbtextures];

float angle = 0.0f;
#define DEBUG

GLenum err;

// Modele de la camera
static float position[] = {0.0f,0.0f,3.0f};
static float aim[] = {0.0f,0.0f,-1.0f};
static float direction[] = {0.0f,0.0f,-1.0f};

// Rotation de la camera
static float yrotation = 0.0f;
static float xrotation = 0.0f;

// Données sur le curseur et la souris
static int xmouseposition = 0;
static int ymouseposition = 0;
static bool buttonpressed = false;

int timebase = 0;

static void quit(void);

// Identifiants d'objets GPU
static GLuint sphereVerticesObjectId;
static GLuint sphereTexCoordsObjectId;
static GLuint sphereNormalsObjectId;
static GLuint sphereFacesObjectId;
static int sphereNbVertices;
static int sphereNbFaces;

// Identifiants de shaders
static GLuint shaderProgramId;

// ------------------------------------------

static bool initExtensions(void)
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Error : " << glewGetErrorString(err) << std::endl;
		return false;
	}

	if(!glewIsSupported("GL_ARB_multitexture"))
	{
		std::cerr << "GL_ARB_multitexture not supported" << std::endl;
		return false;
	}

	return true;
}

static bool initGL(int argc, char **argv)
{
	// Définitions d'états OpenGL
	glClearColor(0.8f,0.8f,0.8f,1.0f);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	// Création des textures
	const char* texNames[] =
	{
		"textures/earth.ppm",
		"textures/clouds.ppm",
		"textures/night.ppm"
	}; 

	glGenTextures(nbtextures, texturesid);

	for(unsigned int i = 0; i < nbtextures; ++i)
	{
		glBindTexture(GL_TEXTURE_2D,texturesid[i]);

		unsigned int tmpwidth, tmpheight;
		unsigned char * image = loadPPM(texNames[i],tmpwidth,tmpheight);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		delete[] image;
	}


	// Vérification d'eventuelles erreurs OpenGL
	err = glGetError();
	if(err != GL_NO_ERROR)
	{
		std::cerr << "Erreur GL : Creation Textures" << std::endl;
		std::cerr << gluErrorString(err) << std::endl;
		return false;
	}
  
	// Chargement des shaders
	GLhandleARB handles[2];
  
	handles[0] = loadShader("shaders/color.vert");
	compileShader(handles[0]);
  
	handles[1] = loadShader("shaders/color.frag");
	compileShader(handles[1]);
  
	shaderProgramId = linkShaders(handles, 2);

	glUseProgram( shaderProgramId );
	glUniform1i( glGetUniformLocation( shaderProgramId, "diffuseTex" ), 0 );
	glUniform1i( glGetUniformLocation( shaderProgramId, "cloudsTex" ), 1 );
	glUniform1i( glGetUniformLocation( shaderProgramId, "nightTex" ), 2 );
	glUniform1i( glGetUniformLocation( shaderProgramId, "normals" ), 3 );

	static GLfloat lightDir[3];
	static GLfloat lightAngle = 0.f;

	lightAngle += 0.001f; lightAngle = fmod(lightAngle, 2 * 3.1415f);	

	lightDir[0] = cos(lightAngle);
	lightDir[2] = sin(lightAngle);
	lightDir[1] = -0.5f;

	GLfloat norm = pow(lightDir[0], 2) + pow(lightDir[1], 2) + pow(lightDir[2], 2);
	norm = 1.f / sqrt(norm);

	lightDir[0] *= norm;
	lightDir[1] *= norm;
	lightDir[2] *= norm;

	glUniform3f( glGetUniformLocation( shaderProgramId, "light" ), lightDir[0], lightDir[1], lightDir[2] );
  
	// ---------------------------------

	//timebase = glutGet(GLUT_ELAPSED_TIME);
	timebase = SDL_GetTicks();

	return true;
}

static void displayGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glRotatef(xrotation,1.0f,0.0f,0.0f);
	glRotatef(yrotation,0.0f,1.0f,0.0f);
	glTranslatef(-position[0],-position[1],-position[2]);

	glPushMatrix();
	glRotatef(angle,0.0,1.0,0.0);

	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, texturesid[2] );

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer( GL_ARRAY_BUFFER, sphereNormalsObjectId );
	glVertexAttribPointer( glGetAttribLocation(shaderProgramId, "normals"), 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0 );
  
	// ----------------------------
	drawSphereDeprecated(1.0, 50, 50);
	// ----------------------------
  
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

#ifdef DEBUG
	err = glGetError();
	if(err!=GL_NO_ERROR)
	{
		std::cerr << "Erreur GL :" << std::endl;
		std::cerr << gluErrorString(err) << std::endl;
		quit();
	}
#endif

	glPopMatrix();

	SDL_GL_SwapBuffers();
}

static void reshapeGL(int newwidth, int newheight)
{
	float ratio = (float)newwidth/(float)newheight;
	glViewport(0, 0, (GLint)newwidth, (GLint)newheight);

	windowwidth = newwidth;
	windowheight = newheight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void idleGL(void)
{
	angle += 0.1f;

	int dt = SDL_GetTicks() - timebase;
	if(dt >= 30)
	{
		timebase += dt - 30;
		displayGL();
	}
}

static void quit(void)
{
	for(unsigned int i = 0; i < nbtextures; ++i)
	{
		if(texturesid[i]!=0)
			glDeleteTextures(1,texturesid+i);
	}
	exit(0);
}

static void keyboardGL(SDL_KeyboardEvent *event)
{
	switch(event->keysym.sym)
	{
	case SDLK_ESCAPE: case SDLK_q:
		quit();
		break;
	case SDLK_w:
		GLint wtype[2];
		glGetIntegerv(GL_POLYGON_MODE,wtype);
		if(wtype[0]==GL_FILL)
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		break;
	default:
		break;
	}
}

static void keyboardSpecialGL(SDL_KeyboardEvent *event)
{
	float step = 0.05f;
	float x1,z1;

	switch(event->keysym.sym)
	{
		case SDLK_UP :
			position[0] += step*direction[0];
			position[2] += step*direction[2];
			break;
		case SDLK_DOWN :
			position[0] -= step*direction[0];
			position[2] -= step*direction[2];
			break;
		case SDLK_LEFT :
			x1 = -direction[2];
			z1 = direction[0];
			position[0] -= step*x1;
			position[2] -= step*z1;
			break;
		case SDLK_RIGHT :
			x1 = -direction[2];
			z1 = direction[0];
			position[0] += step*x1;
			position[2] += step*z1;
			break;
		case SDLK_PAGEUP :
			position[1] += step;
			direction[1] += step;
			break;
		case SDLK_PAGEDOWN :
			position[1] -= step;
			direction[1] -= step;
			break;
		default:
			break;
	}
}

static void mouseGL(SDL_MouseButtonEvent *event)
{
	if(event->button == int(SDL_BUTTON_LEFT) && event->state == int(SDL_PRESSED))
	{
		xmouseposition = event->x;
		ymouseposition = event->y;
		buttonpressed = 1;
	}

	if(event->button == int(SDL_BUTTON_LEFT) && event->state == int(SDL_RELEASED))
	{
		buttonpressed = 0;
	}
}


static void motionGL(SDL_MouseMotionEvent *event)
{
	if(buttonpressed)
	{
		yrotation += (event->x - xmouseposition)/10.0f;
		xrotation += (event->y - ymouseposition)/10.0f;
		aim[0]=float(sin(yrotation*M_PI/180.0f));
		aim[1]=float(-sin(xrotation*M_PI/180.0f));
		aim[2]=float(-cos(yrotation*M_PI/180.0f));
		direction[0]=float(sin(yrotation*M_PI/180.0f));
		direction[2]=float(-cos(yrotation*M_PI/180.0f));
		xmouseposition = event->x;
		ymouseposition = event->y;
	}
}

int main(int argc, char** argv)
{
	SDL_Event event;
	SDL_Surface *screen;
	int done = 0;
    int fps, last,now;
	
	// Initialisation de l'affichage SDL
	if(SDL_Init(SDL_INIT_VIDEO)!=0) {
		std::cerr << "Probleme pour initialiser SDL : " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Nommage de la fenetre
	SDL_WM_SetCaption("TD 1 - Earth Shading", NULL);

	// Activation du double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Creation de la fenetre
	screen = SDL_SetVideoMode(windowwidth, windowheight, 32, SDL_OPENGL);
	if(screen == NULL)
		done = 1;
	
	// Initialisation de GLEW
	initExtensions();

	initGL(argc, argv);
	
  	last = SDL_GetTicks() / 1000;
    fps = 0;
    	
    // Evenement reshape initial
    reshapeGL(windowwidth, windowheight);
    	
  	// Boucle evenementielle
	while(!done)
	{
		// Detection et traitement des evenements
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_VIDEORESIZE:
					reshapeGL(event.resize.w, event.resize.h);
					break;
				case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
					mouseGL(&(event.button));
					break;
				case SDL_KEYDOWN:
					keyboardGL(&(event.key));
					keyboardSpecialGL(&(event.key));
					break;
				case SDL_MOUSEMOTION:
					motionGL(&(event.motion));
					break;
				default:
					break;
			}
		}
		
		idleGL();
		
		fps++;
		now = SDL_GetTicks()/1000;
		if(now != last)
		{
			std::cout << "FPS: " << fps/(now-last) << std::endl;
			last = now;
			fps = 0;
		}
		
		displayGL();
	}
	
	SDL_Quit();
    
	return EXIT_SUCCESS;
}