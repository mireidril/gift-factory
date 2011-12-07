#include "Scene.hpp"
#include "Object.hpp"
#include "ShaderManager.hpp"
#include "Camera.hpp"
#include "Application.hpp"
#include "Spline.hpp"

Scene::Scene(Application* parentApp)
{
	m_parentApp = parentApp;

	//tests spline 
	std::vector<Spline::PointSpline> vertices;
	Spline::PointSpline v1;
	v1.position[0] = 0;
	v1.position[1] = 0;
	v1.position[2] = 12;
	v1.nbFrames = 1;
	v1.yaw = 0;
	v1.debutRotation = false;
	v1.finRotation = false;
	vertices.push_back(v1);

	Spline::PointSpline v2;
	v2.position[0] = -2;
	v2.position[1] = 0;
	v2.position[2] = 9;
	v2.nbFrames = 1;
	v2.yaw = 0;
	v2.debutRotation = true;
	v2.finRotation = false;
	vertices.push_back(v2);

	Spline::PointSpline v3;
	v3.position[0] = 0;
	v3.position[1] = 0;
	v3.position[2] = 6;
	v3.nbFrames = 1;
	v3.yaw = -90;
	v3.debutRotation = false;
	v3.finRotation = true;
	vertices.push_back(v3);

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
	vertices.push_back(v4);

	Spline::PointSpline v5;
	v5.position[0] = 3;
	v5.position[1] = 0;
	v5.position[2] = 2;
	v5.nbFrames = 1;
	v5.yaw = 20;
	v5.debutRotation = true;
	v5.finRotation = false;
	vertices.push_back(v5);

	Spline::PointSpline v6;
	v6.position[0] = 1;
	v6.position[1] = 0;
	v6.position[2] = 1;
	v6.nbFrames = 1;
	v6.yaw = 45;
	v6.debutRotation = false;
	v6.finRotation = true;
	vertices.push_back(v6);

	_spline = new Spline(vertices, 5000);
}

Scene::~Scene()
{}

void Scene::init()
{
	//ShaderManager::getInstance()->addShaders("color", false);
	//ShaderManager::getInstance()->addShaders("texture2D", false);
	initFBODatas();
	loadObj("SET_test");
	//loadObj("SET_scene1");

	
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

	for(int n = 0; n<numObjects; n++){
		getline(stream, line); // get the ligne
		buffer.clear(); buffer.str(line);
		std::string name; buffer >> name; // Read the name of the object
		std::string fileName = "./objects/"+name+".obj";

		Object * objLoaded = new Object(fileName.c_str());

		float * transformMat = new float[16];
		for(j=0; j<16; j++) buffer >> transformMat[j];
		//transformMat[0] *= objects[i].scale[0]; 
		//transformMat[5] *= objects[i].scale[1]; 
		//objects[i].transformArray[10] *= objects[i].scale[2];	// Apply object scale

		//matrix4 rot, rotY; math::makeRotYMatrix(rotY, yaw);	// Apply yaw rotation
		//math::multiplyMatrix(rot, objects[i].transformArray, rotY);
		//math::copyMatrix(objects[i].transformArray, rot);

		//objects[i].numTransforms = 1;
		//objects[i].transformArraySize = 1;
		objLoaded->setTransformMat( transformMat );

		objects.push_back( objLoaded );
	}
}

void Scene::update(){
	
	//m_camera->updateView(); //TESTS CELINE
}

/*
void Scene::render()
{
	// ===== FBO rendering =====
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboDatas.fbo);
	glDisable(GL_TEXTURE_2D);

	// ===== 1ere passe ===== 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboDatas.colorTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fboDatas.depthTexture, 0);

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
	//ShaderManager::actualShader = NULL;

	// Transformations de positionnement de la camera
	GLfloat position[] = {0.0, 0.0, 4.0};
	GLfloat aim[] = {0.0, 0.0, 0.0};
	GLfloat up[] = {0.0, 1.0, 0.0}; // Vector pointing over the camera
	m_camera->lookAt(position, aim, up);
	view = getCamera()->getView();
	

	// ===== 2eme passe ===== 
	// ===== Screen rendering =====

	glPushMatrix();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboDatas.littleColorTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fboDatas.littleDepthTexture, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glViewport(0, 0, Application::windowWidth, Application::windowHeight);
		glViewport(0, 0, m_fboDatas.width/4, m_fboDatas.height/4);
        
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		GLhandleARB idShaderFBO = ShaderManager::getInstance()->getShaderProgramId("passe2");
		glUseProgram( idShaderFBO );
		
		glUniformMatrix4fv(glGetUniformLocation(idShaderFBO, "view"), 1, GL_FALSE, view);
		
		GLfloat m_transformMat[16] = { 2.0*(float)Application::windowWidth/(float)Application::windowHeight, 0.0, 0.0, 0.0,  0.0, 2.0, 0.0, 0.0,  0.0, 0.0, 1.0, -1.0,  0.0, 0.0, 0.0, 1.0};
		glUniformMatrix4fv(glGetUniformLocation( idShaderFBO, "model" ), 1, GL_TRUE, m_transformMat);

		glActiveTexture(GL_TEXTURE0);
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_fboDatas.colorTexture);
		glUniform1i( glGetUniformLocation( idShaderFBO, "FBOtex" ), 0 );
		
		drawSquare();

		glBindTexture(GL_TEXTURE_2D, NULL);
		//objects[0]->draw(view); // TROUVER UNE SOLUTION !!!		
	
	glPopMatrix();

	// ===== Désactivation du FBO =====
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboDatas.fbo);
	glDisable(GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glPushMatrix();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Application::windowWidth, Application::windowHeight);
        
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		idShaderFBO = ShaderManager::getInstance()->getShaderProgramId("passe3");
		glUseProgram( idShaderFBO );
		
		glUniformMatrix4fv(glGetUniformLocation(idShaderFBO, "view"), 1, GL_FALSE, view);
		
		GLfloat m_transformMat2[16] = { 2.0*(float)Application::windowWidth/(float)Application::windowHeight, 0.0, 0.0, 0.0,  0.0, 2.0, 0.0, 0.0,  0.0, 0.0, 1.0, 0.0,  0.0, 0.0, 0.0, 1.0};
		glUniformMatrix4fv(glGetUniformLocation( idShaderFBO, "model" ), 1, GL_TRUE, m_transformMat2);


		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_fboDatas.colorTexture);
		glUniform1i( glGetUniformLocation( idShaderFBO, "FBOtex" ), 0 );

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_fboDatas.littleColorTexture);
		glUniform1i( glGetUniformLocation( idShaderFBO, "littleFBOtex" ), 1 );
		
		drawSquare();
        
		glBindTexture(GL_TEXTURE_2D, NULL);
		//objects[0]->draw(view); // TROUVER UNE SOLUTION !!!		
	
	glPopMatrix();
	
	SDL_GL_SwapBuffers();
}
*/

void Scene::render()
{
	//glPushMatrix();
	//Draw here
	
	//TODO : envoyer au shader actuellement utilise
	GLfloat* view = m_parentApp->getCamera()->getView();
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::getInstance()->getShaderProgramId("texture2D"), "view"), 1, GL_FALSE, view);

	//glUniform3f( glGetUniformLocation( earthProgram, "light" ), lightDir[0], lightDir[1], lightDir[2] );
	//glutSolidTeapot( 1.0 );
	for(unsigned int i = 0; i<objects.size() ; i++){
		objects[i]->draw(view);
	}
	
	glPopMatrix();
	//FIN TODO
}

void Scene::drawSquare(){
	//glTranslatef( 0.0f, 0.0f, -2.0f );

	glScalef((float)Application::windowWidth/(float)Application::windowHeight, 1.0, 1.0);	
	//glScalef((float)m_fboDatas.width/(float)m_fboDatas.height, 1.0, 1.0);	

	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f ); 
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, 0.0f ); 
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f( 1.0f, 1.0f, 0.0f ); 
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, 1.0f, 0.0f ); 
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
	glEnd();
}