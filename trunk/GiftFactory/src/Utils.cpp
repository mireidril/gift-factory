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

// To normalize a vector
void normalize (GLfloat * a)
{
	GLfloat norm=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
	if (norm!=0.0)
	{
	    a[0]/=norm;
	    a[1]/=norm;
	    a[2]/=norm;
	}
}

GLfloat getNorm (GLfloat * a){
	return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}


// To get the vector product
void vectorProduct (GLfloat * a, GLfloat * b, GLfloat * result)
{
	result[0]=a[1]*b[2] - a[2]*b[1];
	result[1]=a[2]*b[0] - a[0]*b[2];
	result[2]=a[0]*b[1] - a[1]*b[0];
}

// Does the multiplication A=A*B : all the matrices are described column-major
void multMatrixBtoMatrixA(GLfloat * A, GLfloat * B)
{
    int i=0; // row index
    int j=0; // column index
    GLfloat temp[16];
    
    for (int iValue=0 ; iValue<16 ; iValue++)
    {
        temp[iValue]=0;
        //j=iValue%4; // if raw-major
        //i=iValue/4; // if raw-major
        i=iValue%4; // if column-major
        j=iValue/4; // if column-major
        for (int k=0 ; k<4 ; k++)
        {
            int indexik=k*4+i;
            int indexkj=j*4+k;
            temp[iValue]+=A[indexik]*B[indexkj];
        }
    }
    
    for (int iValue=0 ; iValue<16 ; iValue++)
        A[iValue]=temp[iValue];
}


// Sets the provided matrix to identity
void setToIdentity(GLfloat * matrix)
{
    GLfloat I[]={1.0, 0.0, 0.0, 0.0, 
                 0.0, 1.0, 0.0, 0.0, 
                 0.0, 0.0, 1.0, 0.0, 
                 0.0, 0.0, 0.0, 1.0};
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        matrix[iMatrixCoord]=I[iMatrixCoord];

}


// Sets the provided matrix to a translate matrix on vector t
void setToTranslate(GLfloat * matrix, GLfloat * t)
{
    GLfloat T[]={1.0,   0.0,   0.0,   0.0,
                 0.0,   1.0,   0.0,   0.0,
                 0.0,   0.0,   1.0,   0.0,
                 t[0],  t[1],  t[2],  1.0}; 
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        matrix[iMatrixCoord]=T[iMatrixCoord];
}


// Sets the provided matrix to a scale matrix by coeficients in s
void setToScale(GLfloat * matrix, GLfloat * s)
{
    GLfloat S[]={s[0], 0.0,  0.0,  0.0,
                 0.0,  s[1], 0.0,  0.0,
                 0.0,  0.0,  s[2], 0.0,
                 0.0,  0.0,  0.0,  1.0};  
    for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
        matrix[iMatrixCoord]=S[iMatrixCoord];
}


// Sets the provided matrix to a rotate matrix of angle "angle", around axis "axis"
void setToRotate(GLfloat * matrix, GLfloat angle, GLfloat * axis)
{
    GLfloat c=cos(angle);
    GLfloat s=sin(angle);
    GLfloat x=axis[0]; 
    GLfloat y=axis[1]; 
    GLfloat z=axis[2];

    if ((x==1.0) && (y==0.0) && (z==0.0))
    {
        GLfloat R[]={1.0, 0.0, 0.0, 0.0, 
                     0.0, c,   s,   0.0, 
                     0.0, -s,  c,   0.0, 
                     0.0, 0.0, 0.0, 1.0};
        for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
            matrix[iMatrixCoord]=R[iMatrixCoord];
    }
    else
    {
        if ((x==0.0) && (y==1.0) && (z==0.0))
        {                    
            GLfloat R[]={c,   0.0, -s,  0.0, 
                         0.0, 1.0, 0.0, 0.0, 
                         s,   0.0, c,   0.0, 
                         0.0, 0.0, 0.0, 1.0};
            for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
                matrix[iMatrixCoord]=R[iMatrixCoord];
        }
        else
        {

            if ((x==0.0) && (y==0.0) && (z==1.0))
            {                                          
                GLfloat R[]={c,   s,   0.0, 0.0, 
                             -s,  c,   0.0, 0.0, 
                             0.0, 0.0, 1.0, 0.0, 
                             0.0, 0.0, 0.0, 1.0};
                for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
                    matrix[iMatrixCoord]=R[iMatrixCoord];
            }
            else
            {
                GLfloat R[]={ (1.0-c)*(x*x-1.0) + 1.0, (1.0-c)*x*y + (z*s),     (1.0-c)*x*z - (y*s),      0.0, 
                              (1.0-c)*x*y - (z*s),     (1.0-c)*(y*y-1.0) + 1.0, (1.0-c)*y*z + (x*s),      0.0, 
                              (1.0-c)*x*z + (y*s),     (1.0-c)*y*z - (x*s),     (1.0-c)*(z*z-1.0) + 1.0,  0.0, 
                              0.0,                     0.0,                     0.0,                      1.0};
                for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
                    matrix[iMatrixCoord]=R[iMatrixCoord];
                std::cout<<"Rotation on non standard axis."<<std::endl;
            }
        }
    }
}