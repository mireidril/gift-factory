#version 120

//varying vec4 OutTexCoord0;
//uniform sampler2D tex2;
uniform sampler2D diffuseTexture;
uniform sampler2D cloudTexture;
uniform sampler2D nightTexture;

void main(){
	vec3 diffuse = texture2D( diffuseTexture, gl_TexCoord[0].st ).rgb;
	vec3 clouds = texture2D( cloudTexture, gl_TexCoord[0].st ).rgb;
	vec3 night = texture2D( nightTexture, gl_TexCoord[0].st ).rgb;

	gl_FragColor = vec4((diffuse + clouds + night), 1.0);
}
