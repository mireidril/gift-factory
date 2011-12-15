#include "ObjectManager.hpp"
#include "Object.hpp"
#include "Scene.hpp"

ObjectManager* ObjectManager::m_pInstance = NULL;

ObjectManager* ObjectManager::getInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new ObjectManager();
	}

	return m_pInstance;
}

void ObjectManager::destroy()
{
	delete m_pInstance;
}

Object* ObjectManager::addObject(Scene* scene, const char* fileName)
{
#ifdef _WIN32
	std::string sFileName("./objects/");
#else
	std::string sFileName("../objects/");
#endif
	sFileName += fileName;
	sFileName += ".obj" ;
	
	Object * objLoaded = new Object(scene, sFileName.c_str());
	Object * sameObject = getObject(fileName);
	if(sameObject == NULL)
	{
		objLoaded->init();
	}
	else
	{
		//It's the same mesh, shader ands textures
		objLoaded->copy(sameObject);
	}


	if(objLoaded != NULL)
	{
		m_mapObject.insert(std::pair<std::string, Object*>(fileName, objLoaded) );
		return objLoaded;
	}
	return NULL;
}

Object* ObjectManager::getObject(const std::string & sObjectName)
{
	std::map< std::string, Object* >::const_iterator it = m_mapObject.find(sObjectName);
	if(it != m_mapObject.end() )
	{
		return (*it).second;
	}
	else
	{
		return NULL;
	}
}

ObjectManager::ObjectManager()
{
	m_mapObject.clear();
}

ObjectManager::~ObjectManager()
{
	m_mapObject.clear();
}
