#include "Scene.hpp"
#include "Object.hpp"
#include "ShaderManager.hpp"
#include "Camera.hpp"

Scene::Scene()
{}

Scene::~Scene()
{}

void Scene::init()
{
	//ShaderManager::getInstance()->addShaders("color", false);
	//ShaderManager::getInstance()->addShaders("texture2D", false);
	
	initFBODatas();
	loadObj("SET_test");
	
	//TODO : fonction setCamera
	m_camera =  new Camera();
}


void Scene::initFBODatas(){

	m_fboDatas = FBODatas();
	m_fboDatas.width = 1024; m_fboDatas.height = 1024;

	// ===== Depth texture =====
	glGenTextures(1, &m_fboDatas.depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboDatas.depthTexture);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_fboDatas.width, m_fboDatas.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// ===== Color texture =====
	glGenTextures(1, &m_fboDatas.colorTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboDatas.colorTexture);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fboDatas.width, m_fboDatas.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// ===== Little Depth texture =====
	glGenTextures(1, &m_fboDatas.littleDepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboDatas.littleDepthTexture);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_fboDatas.width/4, m_fboDatas.height/4, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// ===== Little Color texture =====
	glGenTextures(1, &m_fboDatas.littleColorTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboDatas.littleColorTexture);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fboDatas.width/4, m_fboDatas.height/4, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
        
	// ===== Framebuffer =====
	glGenFramebuffers(1, &m_fboDatas.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboDatas.fbo);
       

	// Attach the buffer and texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fboDatas.depthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboDatas.colorTexture, 0);
 
	// Check errors
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE) std::cerr << "Couldn't create frame buffer, error : " << status << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Scene::loadObj(const std::string setFile)
{
	unsigned int i, j;
	std::stringstream buffer; std::string line;
	std::fstream stream(setFile.c_str(), std::ios::in);
	int numObjects = 0;
	
	while(std::getline(stream, line)) numObjects++;
	
	stream.clear(); stream.seekg(0, std::ios::beg);	// Rewind the file

	for(unsigned int n = 0; n<numObjects; n++){
		getline(stream, line); // get the ligne
		buffer.clear(); buffer.str(line);
		std::string name; buffer >> name; // Read the name of the object
		std::string fileName = "./objects/"+name+".obj";

		Object * objLoaded = new Object(fileName.c_str());

		float * transformMat = new float[16];
		for(j=0; j<16; j++) buffer >> transformMat[j];
		objLoaded->setTransformMat( transformMat );

		objects.push_back( objLoaded );
	}
}

void Scene::update(){
	m_camera->moveForward();
}


void Scene::render()
{
	// ===== FBO rendering =====
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboDatas.fbo);
	glDisable(GL_TEXTURE_2D);

	// ===== 1ere passe ===== 

	glPushMatrix();
		//glClearColor(0.8f,0.8f,0.8f,1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 
		glViewport(0, 0, m_fboDatas.width, m_fboDatas.height);

		// Dessin de la boule indiquant la position de la lumiere
		GLfloat* view = getCamera()->getView();
		//glUniformMatrix4fv(glGetUniformLocation(ShaderManager::getInstance()->getShaderProgramId("texture2D"), "view"), 1, GL_FALSE, view);

		for(unsigned int i = 0; i<objects.size() ; i++){
			objects[i]->draw(view);
		}
	glPopMatrix();
  
	// Desactivation du shader
	glUseProgram(0);

	// ===== Désactivation du FBO =====
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboDatas.fbo);
	glDisable(GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Transformations de positionnement de la camera
	GLfloat position[] = {0.0, 0.0, 4.0};
	GLfloat aim[] = {0.0, 0.0, 0.0};
	GLfloat up[] = {0.0, 1.0, 0.0}; // Vector pointing over the camera
	m_camera->lookAt(position, aim, up);
	view = getCamera()->getView();

	// ===== Screen rendering =====
	glPushMatrix();
        
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Application::windowWidth, Application::windowHeight);
        
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_fboDatas.colorTexture);
		
		drawSquare();
        
		glBindTexture(GL_TEXTURE_2D, NULL);
		objects[0]->draw(view); // TROUVER UNE SOLUTION !!!		
	
	glPopMatrix();
	
	SDL_GL_SwapBuffers();
}


void Scene::drawSquare(){
	glTranslatef( 0.0f, 0.0f, -2.0f );

	glScalef((float)Application::windowWidth/(float)Application::windowHeight, 1.0, 1.0);	

	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f ); 
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, 0.0f ); 
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f( 1.0f, 1.0f, 0.0f ); 
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, 1.0f, 0.0f ); 
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
	glEnd();
}