#include "Scene.hpp"
#include "Object.hpp"
#include "ShaderManager.hpp"

Scene::Scene()
{}

Scene::~Scene()
{}

void Scene::init()
{
	ShaderManager::getInstance()->addShaders("color", false);
	ShaderManager::getInstance()->addShaders("texture2D", false);
	Object * cube = new Object("./objects/planete.obj");
	objects.push_back(cube);
}

void Scene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw here

	//TODO : TEST à virer
//	glUseProgramObjectARB(ShaderManager::getInstance()->getShaderProgramId("texture2D") ); // Maintenant dans le draw de chaque objet -> a améliorer ?
	glPushMatrix();
	glTranslatef(0.f, 0.f, -3.f);
	
	//glutSolidTeapot( 1.0 );
	for(unsigned int i = 0; i<objects.size() ; i++){
		objects[i]->draw();
	}
	
	glPopMatrix();
	//FIN TODO
	SDL_GL_SwapBuffers();
}