#version 120

//varying vec4 OutTexCoord0;
//uniform sampler2D tex2;
uniform sampler2D diffuseTexture;
uniform sampler2D cloudTexture;
uniform sampler2D nightTexture;
uniform mat4 viewMat;

uniform mat4 matTransform;

void main(){
	vec3 diffuse = texture2D( diffuseTexture, gl_TexCoord[0].st ).rgb;
	vec3 clouds = texture2D( cloudTexture, gl_TexCoord[0].st ).rgb;
	vec3 night = texture2D( nightTexture, gl_TexCoord[0].st ).rgb;
	
	if (viewMat[0][0] == 1.0 && viewMat[0][1] == 0.0 && transpose(viewMat)[3][2] == -3.0){
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		//gl_FragColor = vec4((diffuse + clouds + night), 1.0);
	}
	else {		
		//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		gl_FragColor = vec4((diffuse + clouds + night), 1.0);
	}

}
