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
	ShaderManager::getInstance()->addShaders("color", false);
	ShaderManager::getInstance()->addShaders("texture2D", false);
	loadObj("SET_test");
	//loadObj("SET_scene1");

	//TODO : fonction setCamera
	m_camera =  new Camera();
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
	m_camera->moveForward();
	//m_camera->updateView(); //TESTS CELINE
}


void Scene::render()
{
	//Draw here
	
	//TODO : envoyer au shader actuellement utilise
	GLfloat* view = getCamera()->getView();
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::getInstance()->getShaderProgramId("texture2D"), "view"), 1, GL_FALSE, view);

	//glUniform3f( glGetUniformLocation( earthProgram, "light" ), lightDir[0], lightDir[1], lightDir[2] );
	//glutSolidTeapot( 1.0 );
	for(unsigned int i = 0; i<objects.size() ; i++){
		objects[i]->draw();
	}
	
	glPopMatrix();
	//FIN TODO
}