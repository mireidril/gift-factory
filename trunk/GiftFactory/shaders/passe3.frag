#version 120

uniform sampler2D FBOtex;
uniform sampler2D littleFBOtex;

void main(){
	//vec3 diffuse = texture2D( FBOtex, gl_TexCoord[0].st ).rgb;
	//vec3 diffuse2 = texture2D( littleFBOtex, gl_TexCoord[0].st ).rgb;
	//gl_FragColor = vec4((diffuse+diffuse2), 1.0);
	
	float t = texture(FBOtex, vec2(gl_TexCoord[0].s, 1.0-gl_TexCoord[0].t)).a;
	gl_FragColor = vec4( texture2D(FBOtex, vec2(gl_TexCoord[0].s, 1.0-gl_TexCoord[0].t)).rgb * (1-t) + t*texture2D(littleFBOtex, gl_TexCoord[0].st).rgb, 1.0);

}