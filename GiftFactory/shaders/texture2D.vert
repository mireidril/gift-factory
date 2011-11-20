#version 120

//uniform mat4 ProjectionModelviewMatrix;
//uniform mat4 ModelviewMatrix;
uniform mat4 matTransform;

//attribute vec3 inVertex;
//attribute vec2 inTexCoords;
//attribute vec3 inNormals;

//varying vec4 OutTexCoord0;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	mat4 matModelViewProjObjet = matTransform * gl_ModelViewProjectionMatrix;
	gl_Position = matModelViewProjObjet *gl_Vertex;// * vec4(inVertex, 1.0);
	
}

