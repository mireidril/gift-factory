#version 120

varying vec2 texCoords;
varying vec3 normals;

uniform sampler2D diffuseTex;
uniform sampler2D cloudsTex;
uniform sampler2D nightTex;

uniform vec3 light;

void main(){
	vec3 diffuse = texture2D( diffuseTex, texCoords ).rgb;
	vec3 clouds = texture2D( cloudsTex, texCoords ).rgb;
	vec3 night = texture2D( nightTex, texCoords ).rgb;

	gl_FragColor = vec4( mix((diffuse + clouds)*max(0, -dot(normals, light)), night, 1.0-max(0, -dot(normals, light))), 1.0);

}
