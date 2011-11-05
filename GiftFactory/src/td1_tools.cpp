#include "td1_tools.hpp"

GLubyte * loadPPM(const char * const fn, unsigned int& w, unsigned int& h)
{
	char head[70];
	int i, j;
	int d;
	GLubyte * img = NULL;

	FILE * f = fopen(fn, "r");
	if(f == NULL)
	{
		fprintf(stderr,"Error in function readPPM : %s doesn't exist\n",fn);
		return 0;
	}

	(void)fgets(head,70,f);

	if(!strncmp(head, "P6", 2) )
	{
		i=0;
		j=0;
		while(i<3)
		{
			(void)fgets(head,70,f);
			if(head[0] == '#')
			{
				continue;
			}
			if(i==0)
				i += sscanf_s(head, "%d %d %d", &w, &h, &d);
			else
				if(i==1)
					i += sscanf_s(head, "%d %d", &h, &d);
				else
					if(i==2)
						i += sscanf_s(head, "%d", &d);
		}
		img = new GLubyte[(size_t)(w) * (size_t)(h) * 3];
	
		if(img==NULL)
		{
			fclose(f);
			return 0;
		}

		(void)fread(img, sizeof(GLubyte), (size_t)w*(size_t)h*3,f);
		fclose(f);
	}
	else
	{
		fclose(f);
		fprintf(stderr,"Error in function readPPM : %s isn't a PPM file\n",fn);
	}

	return img;
}

void drawSphereDeprecated(const float& radius, const unsigned int& stepx, const unsigned int& stepy)
{
	float anglex = float(2 * M_PI) / float(stepx);
	float angley = float(M_PI) / float(stepy);

	float texx = 1.0f / float(stepx);
	float texy = 1.0f / float(stepy);

	for(unsigned int i = 0; i < stepy; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(unsigned int j = 0; j < stepx + 1; ++j)
		{
			glTexCoord2f(texx*j,(texy)*i);
			float x = radius*sin(angley*i)*sin(anglex*j);
			float y = radius*cos(angley*i);
			float z = radius*sin(angley*i)*cos(anglex*j);
			float norm = sqrtf(x*x+y*y+z*z);
			glNormal3f(x/norm,y/norm,z/norm);
			glVertex3f(x, y, z);
			x = radius*sin(angley*(i+1))*sin(anglex*j);
			y = radius*cos(angley*(i+1));
			z = radius*sin(angley*(i+1))*cos(anglex*j);
			glTexCoord2f(texx*j,(texy)*(i+1));
			glNormal3f(x/norm,y/norm,z/norm);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void buildSphereObjects(const float& radius, const unsigned int& stepx, const unsigned int& stepy, GLuint &verticesObjId, GLuint &texCoordsObjId, GLuint &normalsObjId, GLuint &facesObjId, int &nbVertices, int &nbFaces)
{	

}