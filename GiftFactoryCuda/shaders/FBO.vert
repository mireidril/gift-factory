#version 120

//uniform mat4 ProjectionModelviewMatrix;
//uniform mat4 ModelviewMatrix;
uniform mat4 model;
uniform mat4 view;


void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * view * model * gl_Vertex;
	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

