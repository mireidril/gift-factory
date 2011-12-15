#version 130

uniform mat4 model;
uniform mat4 view;

varying vec3 posVertex;
varying vec2 texCoords;
varying vec3 normals;


void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	posVertex = gl_Vertex.xyz;
 	texCoords = gl_TexCoord[0].xy;
	normals =  gl_Normal;

	gl_Position = gl_ProjectionMatrix * view * model * gl_Vertex;
}

