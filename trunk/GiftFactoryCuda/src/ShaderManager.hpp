#ifndef __SHADER_MANAGER_HPP__
#define __SHADER_MANAGER_HPP__

#include "Utils.hpp"

#define MAX_LOG_STRING 1024
#define ERROR_VALUE 0

// Class (singleton) where the shaders are loaded and stored
class ShaderManager
{
	public :
		static ShaderManager* getInstance();
		static void destroy();

		GLhandleARB addShaders(const char* fileName, bool bGeometryShader);

		GLhandleARB loadShader(const std::string & filename);
		GLhandleARB compileShader(GLhandleARB object);
		GLhandleARB linkShaders(GLhandleARB* object, const unsigned int& nb);

		GLhandleARB getShaderProgramId(const std::string & sShaderName);

		static GLhandleARB actualShader;

	private :
		ShaderManager();
		~ShaderManager();

		static ShaderManager* m_pInstance;
		
		std::map< std::string, GLhandleARB > m_mapShaderProgramId;
		char logstring[MAX_LOG_STRING];

		std::string m_sExtensions[3];
};

#endif
