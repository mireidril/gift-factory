#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "Utils.hpp"
#include "Object.hpp"

class Object;

class Scene
{
	public :
		Scene();
		~Scene();

		void init();
		void render();

	private :
		//Objets id
		std::vector<Object*> objects;
};

#endif
