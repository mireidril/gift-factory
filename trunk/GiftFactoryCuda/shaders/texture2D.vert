#version 120

//uniform mat4 ProjectionModelviewMatrix;
//uniform mat4 ModelviewMatrix;
uniform mat4 model;
uniform mat4 view;

//attribute vec3 inVertex;
//attribute vec2 inTexCoords;
//attribute vec3 inNormals;

//varying vec4 OutTexCoord0;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * view * model * gl_Vertex;
}

