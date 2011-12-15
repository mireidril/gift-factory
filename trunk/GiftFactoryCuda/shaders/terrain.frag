#version 130

// Uniforms
uniform sampler2D S2DblendMap;
uniform sampler2D S2Dgrass;
uniform sampler2D S2Dpath;
uniform sampler2D S2Drock;

varying vec3 posVertex;
varying vec2 texCoords;
varying vec3 normals;

uniform vec3 posLight;
uniform vec3 posCamera;

uniform vec4 l_diffuse;
uniform vec4 l_ambient;
uniform vec4 l_specular;

uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shine;

uniform mat4 matTransform;

void main(){
	
	vec2 fUV = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t);
	vec4 blendMapColor = texture(S2DblendMap, fUV);
	vec4 grassColor = texture(S2Dgrass, fUV);
	vec4 pathColor = texture(S2Dpath, fUV);
	vec4 rockColor = texture(S2Drock, fUV);
	
	vec4 	blendColor = mix(grassColor, rockColor, blendMapColor.b);
			blendColor = mix(blendColor, pathColor, blendMapColor.r);
	
	
	float kc = 1.0;
	float kl = 0.0;
	float kq = 0.0;

	vec3 L = posLight - posVertex;

	float d = sqrt( pow(L.x, 2) + pow(L.y, 2) + pow(L.z, 2) );
	vec3 normLight = L / d;

	float sDot = dot(normLight, normals) * 2.0;
	vec3 s = vec3( normLight.x + sDot * normals.x, normLight.y + sDot * normals.y, normLight.z + sDot * normals.z );
	
	vec3 E = posCamera - posVertex;

	vec4 ambientF = vec4( (ambient.rgb * l_ambient.rgb) , 1.0 ) ;
	vec4 diffuseF = max( dot(normLight,normals), 0) * vec4( diffuse.xyz * l_diffuse.xyz, 1 )  ;
	vec4 specularF = pow(max( dot(s, E), 0), shine) * vec4( (specular.rgb * l_specular.rgb) , 1.0 ) ;

	vec4 colorTable;
	float sumInt = dot(normLight, normals);
	if( sumInt < 0.05 )
		colorTable = vec4( 0.4, 0.4, 0.4, 1.0);	
	else if( sumInt < 0.3 && sumInt >= 0.05 )
		colorTable = vec4( 0.6, 0.6, 0.6, 1.0);	
	else if( sumInt < 0.6 && sumInt >= 0.3)
		colorTable = vec4( 0.8, 0.8, 0.8, 1.0);
	else if( sumInt <= 0.9 && sumInt >= 0.6 )
		colorTable = vec4( 0.9, 0.9, 0.9, 1.0);
	else if( sumInt >= 0.9)
		colorTable = vec4( 1.0, 1.0, 1.0, 1.0);
		
	gl_FragColor = colorTable * blendColor;
	
	//gl_FragColor = blendColor;// * light;
}
