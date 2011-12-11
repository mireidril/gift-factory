#version 120

uniform sampler2D FBOtex;

void main(){
	//vec3 diffuse = texture2D( FBOtex, gl_TexCoord[0].st ).rgb;
	//gl_FragColor = vec4(diffuse, 1.0);

	float[49] matGauss;
	float PI = 3.14159265358979323846264;
	float somme = 0;

	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	int cpt = 0;
	for( int y=-3; y<4; y++){
		for( int x=-3; x<4; x++){
			matGauss[cpt] = (1/(sqrt(2*PI)*pow(texture(FBOtex, gl_TexCoord[0].st).a,2.0)))*exp(-((x*x+y*y)/(2*pow(texture(FBOtex, gl_TexCoord[0].st).a,2.0))));
			color += vec4(texture(FBOtex, gl_TexCoord[0].st + vec2(x/1024, y/1024) ).rgb, 1.0) * matGauss[cpt];
			somme += matGauss[cpt];
			cpt++;
		}
	}
	gl_FragColor=vec4(1.0,0.0, 0.0, 1.0);//color/somme;
}