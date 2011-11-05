uniform sampler2D diffuseTex;
uniform sampler2D cloudsTex;
uniform sampler2D nightTex;

varying vec3 normal;

uniform vec3 light;

void main (void)
{
	vec3 earth = texture2D(diffuseTex, gl_TexCoord[0].xy).rgb;
	vec3 cloud = texture2D(cloudsTex, gl_TexCoord[0].xy).rgb;
	vec3 night = texture2D(nightTex, gl_TexCoord[0].xy).rgb;

	//gl_FragColor = vec4( earth + cloud + night, 1.0);
	gl_FragColor = vec4( mix((earth + cloud)*max(0, -dot(normal, light)), night, 1.0-max(0, -dot(normal, light))), 1.0);
}
