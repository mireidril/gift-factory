#version 120

attribute vec3 inVertex;
attribute vec2 inTexCoords;
attribute vec3 inNormals;

uniform mat4 modelView, projection;

varying vec2 texCoords;
varying vec3 normals;

void main(){
	texCoords = inTexCoords;
	normals = inNormals;

	gl_Position = projection * modelView * vec4(inVertex, 1);
}
