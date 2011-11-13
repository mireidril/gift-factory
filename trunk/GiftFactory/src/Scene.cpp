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
}

void Scene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw here

	//TODO : TEST à virer
	glUseProgramObjectARB(ShaderManager::getInstance()->getShaderProgramId("color") );
	glPushMatrix();
	glTranslatef(0.f, 0.f, -3.f);
	glutSolidTeapot( 1.0 );
	glPopMatrix();
	//FIN TODO
	SDL_GL_SwapBuffers();
}