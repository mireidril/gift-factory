#version 120

//uniform mat4 ProjectionModelviewMatrix;
//uniform mat4 ModelviewMatrix;

//attribute vec3 inVertex;
//attribute vec2 inTexCoords;
//attribute vec3 inNormals;

//varying vec4 OutTexCoord0;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix *gl_Vertex;// * vec4(inVertex, 1.0);
	
}

