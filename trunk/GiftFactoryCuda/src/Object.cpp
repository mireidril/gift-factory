#include "Object.hpp"
#include "ShaderManager.hpp"
#include "Application.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Spline.hpp"

Object::Object(Scene * scene, const char* filename, bool enableTextures)
: m_parentScene(scene)
, m_uiNbTextures(0)
, m_iTextureIds(NULL)
, m_sShaderName("")
, objFileName(filename)
, g_enableTextures(enableTextures)
, m_counter(0)
, m_period( 180 )
, m_spline(NULL)
, m_splineEnded(false)
{
	m_shaderManager = ShaderManager::getInstance();
	m_transformMat = NULL;
	
	m_diffuse[0] = 0.f;
	m_diffuse[1] = 0.f;
	m_diffuse[2] = 1.f;
	m_diffuse[3] = 1.f;

	m_ambient[0] = 1.f;
	m_ambient[1] = 0.f;
	m_ambient[2] = 0.f;
	m_ambient[3] = 1.f;

	m_specular[0] = 0.1f;
	m_specular[1] = 0.1f;
	m_specular[2] = 0.1f;
	m_specular[3] = 1.f;

	m_shininess = 2.2f;
}

Object::~Object()
{
	//Textures deletion ?
	
}

void Object::init()
{
	// Mesh initialisation
	std::cout << "obj to load : " << objFileName << std::endl;
	g_model = new ModelOBJ;
	if (!g_model->import(objFileName))
	{
		std::cout << "Failed to load model " << std::endl;
	}
	else 
		g_model->normalize();

	const ModelOBJ::Material *pMaterial = (g_model->getMesh(0)).pMaterial;

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
			if(objFileName == "./objects/terrain.obj" || objFileName == "../objects/terrain.obj" ){
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}else{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
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

void Object::copy(Object * sameObj)
{
	// Mesh initialisation
	g_model = sameObj->g_model;

	const ModelOBJ::Material *pMaterial = (g_model->getMesh(0)).pMaterial;

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

	glUniformMatrix4fv(glGetUniformLocation(m_uiShaderId, "view"), 1, GL_FALSE, view);
		
	glUniform1f(glGetUniformLocation(m_uiShaderId, "focalDistance"), Camera::focalDistance);
	glUniform1f(glGetUniformLocation(m_uiShaderId, "focalRange"), Camera::focalRange);
	
	//Light information
	
	glUniform3fv(glGetUniformLocation(m_uiShaderId, "posCamera"), 1, m_parentScene->getParentApplication()->getCamera()->getPosition());
	if( m_parentScene->m_lightPosition != NULL)
	{
		glUniform3fv(glGetUniformLocation(m_uiShaderId, "posLight"), 1, m_parentScene->m_lightPosition);

		glUniform4fv(glGetUniformLocation(m_uiShaderId, "diffuse"), 1, m_diffuse);
		glUniform4fv(glGetUniformLocation(m_uiShaderId, "ambient"), 1, m_ambient);
		glUniform4fv(glGetUniformLocation(m_uiShaderId, "specular"), 1, m_specular);
		glUniform1f( glGetUniformLocation(m_uiShaderId, "shine" ), m_shininess );

		glUniform4fv(glGetUniformLocation(m_uiShaderId, "l_diffuse"), 1, m_parentScene->m_lightDiffuse);
		glUniform4fv(glGetUniformLocation(m_uiShaderId, "l_ambient"), 1, m_parentScene->m_lightAmbient);
		glUniform4fv(glGetUniformLocation(m_uiShaderId, "l_specular"), 1, m_parentScene->m_lightSpecular);
	}
	
	const ModelOBJ::Mesh *pMesh = 0;
	const ModelOBJ::Material *pMaterial = 0;
	const ModelOBJ::Vertex *pVertices = 0;
	//float modelViewProj[16];

	glDisable(GL_BLEND);
    
	for (int i = 0; i < g_model->getNumberOfMeshes(); ++i)
	{
		pMesh = &(g_model->getMesh(i));
		pMaterial = pMesh->pMaterial;
		pVertices = g_model->getVertexBuffer();

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

			if(strcmp("tinselShader", m_sShaderName) == 0)
			{
				if(m_counter/m_period >= 3)
				{
					m_counter = 0;
				}
				else
				{
					m_counter++;
				}

				glUniform1i( glGetUniformLocation( m_uiShaderId, "actualText" ), m_counter/m_period );
			}	  	
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}

		if (g_model->hasPositions())
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, g_model->getVertexSize(), g_model->getVertexBuffer()->position);
		}

		if (g_model->hasTextureCoords())
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, g_model->getVertexSize(), g_model->getVertexBuffer()->texCoord);
		}

		if (g_model->hasNormals())
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, g_model->getVertexSize(), g_model->getVertexBuffer()->normal);
		}


		// We draw the line if the shader is the toonShader ->doesn't work
		/*if(strcmp("toonShader", m_sShaderName) == 0)
		{
			GLuint simpleShaderUid = m_shaderManager->getShaderProgramId("simpleShader");
			if(simpleShaderUid == ERROR_VALUE)
				simpleShaderUid = m_shaderManager->addShaders("simpleShader", false);
			glUniformMatrix4fv(glGetUniformLocation(simpleShaderUid, "model" ), 1, GL_TRUE, m_transformMat);
			glUniformMatrix4fv(glGetUniformLocation(simpleShaderUid, "view"), 1, GL_FALSE, view);

			glUseProgramObjectARB(simpleShaderUid);
			//glLineWidth(10.f);
			//glEnable(GL_LINE_SMOOTH);
			//glEnable(GL_CULL_FACE);

			//glPolygonMode(GL_BACK, GL_LINE);
			//glDepthFunc(GL_LEQUAL);
			
			//glCullFace(GL_FRONT);

			glDrawElements(GL_TRIANGLES, pMesh->triangleCount * 3, GL_UNSIGNED_INT, g_model.getIndexBuffer() + pMesh->startIndex);

			//glDisable(GL_LINE_SMOOTH);
			//glDisable(GL_CULL_FACE);

			glUseProgramObjectARB(m_uiShaderId);
			//glCullFace(GL_BACK);
			//glPolygonMode(GL_FRONT, GL_FILL);
		}*/		

		glDrawElements(GL_TRIANGLES, pMesh->triangleCount * 3, GL_UNSIGNED_INT, g_model->getIndexBuffer() + pMesh->startIndex);

		if (g_model->hasNormals())
			glDisableClientState(GL_NORMAL_ARRAY);

		if (g_model->hasTextureCoords())
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (g_model->hasPositions())
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

void Object::setSpline(Spline* spline){
	m_spline = spline;
}

void Object::move(){
	if (m_spline != NULL){
		if (!m_splineEnded){
			if (!m_spline->moveForward()){
				m_splineEnded = true;
				//std::cout<<"spline ended"<<std::endl;
			}
			float* transformMat = getTransformMat();
			Spline::PointSpline splinePos = m_spline->getCurrentPosition();
			transformMat[3] = splinePos.position[0];
			transformMat[7] = splinePos.position[1];
			transformMat[11] = splinePos.position[2];
			//std::cout<<objFileName<<" "<<transformMat[3]<<" "<<transformMat[7]<<" "<<transformMat[11]<<std::endl;
			setTransformMat(transformMat);
		}
	}
}