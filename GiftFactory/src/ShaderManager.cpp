#include "ShaderManager.hpp"

ShaderManager* ShaderManager::m_pInstance = NULL;

ShaderManager* ShaderManager::getInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new ShaderManager();
	}

	return m_pInstance;
}

void ShaderManager::destroy()
{
	delete m_pInstance;
}

GLhandleARB ShaderManager::addShaders(const char* fileName,  bool bGeometryShader)
{
	std::string sFileName("shaders/");
	sFileName += fileName;

	unsigned int nbShaderFiles;
	GLhandleARB * shaders;
	if(bGeometryShader)
	{
		nbShaderFiles = 3;
	}
	else
	{
		nbShaderFiles = 2;
	}
	shaders = new GLhandleARB[nbShaderFiles];
	for(unsigned int i = 0; i < nbShaderFiles; ++i)
	{
		shaders[i] = loadShader( sFileName + m_sExtensions[i] );
		compileShader(shaders[i]);
	}
	
	GLhandleARB programId = linkShaders( shaders, nbShaderFiles );
	if(programId != ERROR_VALUE)
	{
		m_mapShaderProgramId.insert(std::pair<std::string, GLhandleARB>(fileName, programId) );
	}
	return programId;
}

GLhandleARB ShaderManager::loadShader(const std::string & sFileName)
{
	if(sFileName == "")
	{
		std::cout << "File name missing" << std::endl;
		return 0;
	}

	bool bVertexshader = false;
	bool bFragmentshader = false;
#ifdef USE_GEOMETRY_SHADER
	bool bGeometryshader = false;
#endif

	if(sFileName.find(".vert") != std::string::npos)
	{
		bVertexshader=true;
	}
	if(sFileName.find(".frag") != std::string::npos)
	{
		bFragmentshader=true;
	}
#ifdef USE_GEOMETRY_SHADER
	else
	{
		if (filestr.find(".geo") != std::string::npos)
		{
			bGeometryshader=true;
		}
	}
#endif
	if(!bVertexshader && !bFragmentshader 
#ifdef USE_GEOMETRY_SHADER 
	&& !bGeometryshader 
#endif
	)
	{
		return 0;
	}

	std::ifstream file(sFileName); 
	if(!file)
	{
		return 0;
	}
  
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	std::string sSource = buffer.str();

	GLcharARB * source = new GLcharARB[4*(sSource.size() / 4+1)];
	if(source == NULL)
	{
		return 0;
	}

	unsigned int i;
	for(i =0; i < buffer.str().size(); ++i)
	{
		source[i] = sSource[i];
	}
	source[i] = '\0';

	GLhandleARB so = 0; 
	if(bVertexshader)
		so = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	if(bFragmentshader)
		so = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);    
#ifdef USE_GEOMETRY_SHADER 
	if(bGeometryshader)
		so = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);  
#endif

	glShaderSourceARB(so, 1,(const GLcharARB**) &source, 0);
	return so;
}

GLhandleARB ShaderManager::compileShader(GLhandleARB object)
{
	if(object == NULL)
		return 0;

	glCompileShaderARB(object);

	int status;
	glGetObjectParameterivARB(object, GL_OBJECT_COMPILE_STATUS_ARB, &status);
	// If the shader compilation failed
	if(status==0)
	{
		int length = 0;
		glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
		if(length>0)
		{
			GLsizei minlength = min(MAX_LOG_STRING, length);
			glGetInfoLogARB(object, minlength, 0, logstring);
			std::cerr << logstring << std::endl;
		}
		return 0;
	}
	return 1;
}

GLhandleARB ShaderManager::linkShaders(GLhandleARB* object, const unsigned int& nb)
{
	if(object == NULL)
		return ERROR_VALUE;

	GLhandleARB programObject = 0;
	programObject = glCreateProgramObjectARB();

	for(unsigned int i = 0; i < nb; ++i)
	{
		if(object[i] > 0)
		{
			glAttachObjectARB(programObject, object[i]);
		}
	}
  
	glLinkProgramARB(programObject);

	int status;
	glGetObjectParameterivARB(programObject, GL_OBJECT_LINK_STATUS_ARB, &status);
	// If the shader linkage failed
	if(status==0)
	{
		int length = 0;
		glGetObjectParameterivARB(programObject, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
		if(length>0)
		{
			GLsizei minlength = min(MAX_LOG_STRING,length);
			glGetInfoLogARB(programObject, minlength, 0, logstring);
			std::cerr << logstring << std::endl;
		}
		return ERROR_VALUE;
	}
	return programObject;
}

GLhandleARB ShaderManager::getShaderProgramId(const std::string & sShaderName)
{
	std::map< std::string, GLhandleARB >::const_iterator it = m_mapShaderProgramId.find(sShaderName);
	if(it != m_mapShaderProgramId.end() )
	{
		return (*it).second;
	}
	else
	{
		return ERROR_VALUE;
	}
}

ShaderManager::ShaderManager()
{
	m_sExtensions[0] = ".vert";
	m_sExtensions[1] = ".frag";
	m_sExtensions[2] = ".geo";
	m_mapShaderProgramId.clear();
}

ShaderManager::~ShaderManager()
{
	m_mapShaderProgramId.clear();
	//delete &logstring;
}