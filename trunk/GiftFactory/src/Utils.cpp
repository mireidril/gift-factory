#include "Utils.hpp"

GLubyte * loadPPM(const char * const fn, unsigned int& w, unsigned int& h)
{
	char head[70];
	int i, j;
	int d;
	GLubyte * img = NULL;

	FILE * f;
	fopen_s(&f, fn, "r");
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