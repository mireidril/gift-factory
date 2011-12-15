#ifndef __OBJECT_MANAGER_HPP__
#define __OBJECT_MANAGER_HPP__

#include "Utils.hpp"

class Object;
class Scene;

// Class (singleton) where the objects are loaded and stored
class ObjectManager
{
	public :
		static ObjectManager* getInstance();
		static void destroy();

		Object* addObject(Scene * scene, const char* fileName);
		Object* getObject(const std::string & sObjectName);

	private :
		ObjectManager();
		~ObjectManager();

		static ObjectManager* m_pInstance;

		std::map< std::string, Object* > m_mapObject;
};

#endif
