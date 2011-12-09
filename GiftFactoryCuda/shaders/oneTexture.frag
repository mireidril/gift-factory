#version 120

//varying vec4 OutTexCoord0;
//uniform sampler2D tex2;
uniform sampler2D diffuseTexture;
uniform mat4 viewMat;

uniform mat4 matTransform;

varying float Blur;

void main(){
	vec3 diffuse = texture2D( diffuseTexture, gl_TexCoord[0].st ).rgb;

	gl_FragColor = vec4((diffuse), Blur);

}