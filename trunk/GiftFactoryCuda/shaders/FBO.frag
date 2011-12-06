#version 120

uniform sampler2D FBOtex;

void main(){
	vec3 diffuse = texture2D( FBOtex, gl_TexCoord[0].st ).rgb;
	if(diffuse.r == 0.0 && diffuse.g == 0.0 && diffuse.b == 0.0)
		gl_FragColor = vec4(0.0,0.0,1.0,1.0);
	else
		gl_FragColor = vec4(diffuse, 1.0);

}
