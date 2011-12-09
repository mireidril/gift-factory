#include "Object.hpp"
#include "ShaderManager.hpp"
#include "Application.hpp"
#include "Camera.hpp"


Object::Object(const char* filename, bool enableTextures)
: m_uiNbTextures(0)
, m_iTextureIds(NULL)
, m_sShaderName("")
, objFileName(filename)
, g_enableTextures(enableTextures)
{
	m_shaderManager = ShaderManager::getInstance();
	m_transformMat = NULL;
	init();
}

Object::~Object()
{
	//Textures deletion ?
	
}

void Object::init()
{
	// Mesh initialisation
	std::cout << "obj to load : " << objFileName << std::endl;
	if (!g_model.import(objFileName))
	{
		SetCursor(LoadCursor(0, IDC_ARROW));
		std::cout << "Failed to load model " << std::endl;
		throw std::runtime_error("Failed to load model.");
	}
	else 
		g_model.normalize();

	const ModelOBJ::Material *pMaterial = (g_model.getMesh(0)).pMaterial;

	//Textures initialization
	if(g_enableTextures){
		glGenTextures(pMaterial->textures.size(), &m_iTextureIds);

		for(unsigned int i = 0 ; i < pMaterial->textures.size() ; i++){

			glBindTexture(GL_TEXTURE_2D,m_iTextureIds+i);
			
			TextureManager::Texture* tex =  pMaterial->textures[i];
			if(tex == 0)
				std::cout << "Erreur texture : la texture n'a pas été chargée préalablement dans model_obj ?" << std::endl;
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->texPicture->w, tex->texPicture->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->texPicture->pixels);

		}
	}

	//Shader initialization
	m_sShaderName = pMaterial->shaderName.c_str();
	m_uiShaderId = m_shaderManager->getShaderProgramId(m_sShaderName);
	if(m_uiShaderId == ERROR_VALUE)
	{
		//false if there are only vertex and fragment shader, or true if there are vertex, fragment and geometry shader
		m_uiShaderId = m_shaderManager->addShaders(m_sShaderName, false);
	}
}

void Object::draw(GLfloat* view)
{
	glUseProgramObjectARB(m_uiShaderId);

	//Draw here
	glUniformMatrix4fv(glGetUniformLocation(m_uiShaderId, "view"), 1, GL_FALSE, view);
		
	glUniform1f(glGetUniformLocation(m_uiShaderId, "focalDistance"), Camera::focalDistance);
	glUniform1f(glGetUniformLocation(m_uiShaderId, "focalRange"), Camera::focalRange);
	
	const ModelOBJ::Mesh *pMesh = 0;
	const ModelOBJ::Material *pMaterial = 0;
	const ModelOBJ::Vertex *pVertices = 0;
	//float modelViewProj[16];
    glDisable(GL_BLEND);
	for (int i = 0; i < g_model.getNumberOfMeshes(); ++i)
	{
		pMesh = &g_model.getMesh(i);
		pMaterial = pMesh->pMaterial;
		pVertices = g_model.getVertexBuffer();

		// modelViewProj[16] = camera.getViewProjMatrix();
		// multMatrixBtoMatrixA( camera.getViewProjMatrix(), objects[i].getModelMatrix());
		// glUniformMatrix4fv(glGetUniformLocation( m_uiShaderId, "modelViewProj" ), 1, GL_TRUE, modelViewProj);
		glUniformMatrix4fv(glGetUniformLocation( m_uiShaderId, "model" ), 1, GL_TRUE, m_transformMat);
		//for(int i = 0 ; i < 16; i++) std::cout << m_transformMat[i] << ", " ;
		//std::cout<< std::endl;
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pMaterial->ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pMaterial->diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pMaterial->specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pMaterial->shininess * 128.0f);
		
		if (g_enableTextures)
		{
			//glEnable(GL_TEXTURE_2D);
			for(unsigned int i = 0 ; i < pMaterial->textures.size() ; i++){
				glActiveTexture( GL_TEXTURE0+i );
				glBindTexture(GL_TEXTURE_2D,m_iTextureIds+i);
				glUniform1i( glGetUniformLocation( m_uiShaderId, pMaterial->textures[i]->shaderUniformName.c_str() ), i );
			}
	  	
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}

		if (g_model.hasPositions())
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, g_model.getVertexSize(), g_model.getVertexBuffer()->position);
		}

		if (g_model.hasTextureCoords())
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, g_model.getVertexSize(), g_model.getVertexBuffer()->texCoord);
		}

		if (g_model.hasNormals())
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, g_model.getVertexSize(), g_model.getVertexBuffer()->normal);
		}

		glDrawElements(GL_TRIANGLES, pMesh->triangleCount * 3, GL_UNSIGNED_INT, g_model.getIndexBuffer() + pMesh->startIndex);

		if (g_model.hasNormals())
			glDisableClientState(GL_NORMAL_ARRAY);

		if (g_model.hasTextureCoords())
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (g_model.hasPositions())
			glDisableClientState(GL_VERTEX_ARRAY);
			
		if (g_enableTextures)
		{
			glEnable(GL_TEXTURE_2D);
			for(unsigned int i = 0 ; i < pMaterial->textures.size() ; i++){
				glActiveTexture( GL_TEXTURE0+i );
				glBindTexture(GL_TEXTURE_2D,0);
			}
		}
	}
}


GLuint Object::LoadTexture(const char *pszFilename)
{
    GLuint id = 0;
    //Bitmap bitmap;
/*
    if (bitmap.loadPicture(pszFilename))
    {
        // The Bitmap class loads images and orients them top-down.
        // OpenGL expects bitmap images to be oriented bottom-up.
        bitmap.flipVertical();

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (g_maxAnisotrophy > 1.0f)
        {
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                g_maxAnisotrophy);
        }

        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bitmap.width, bitmap.height,
            GL_BGRA_EXT, GL_UNSIGNED_BYTE, bitmap.getPixels());
    }
*/
    return id;
}
