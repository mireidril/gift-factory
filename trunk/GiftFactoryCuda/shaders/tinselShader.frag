#version 120

varying vec3 posVertex;
varying vec2 texCoords;
varying vec3 normals;

uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;

uniform mat4 viewMat;
uniform int actualText;

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

varying float Blur;

void main(){
	vec3 texture;
	if(actualText == 0)
		texture = texture2D( diffuseTexture1, gl_TexCoord[0].st ).rgb;
	else if(actualText == 1)
		texture = texture2D( diffuseTexture2, gl_TexCoord[1].st ).rgb;
	else
		texture = texture2D( diffuseTexture3, gl_TexCoord[2].st ).rgb;
	
	/*if(actualText == 0)
		texture = vec3(1.0, 0.0, 0.0);
	else if(actualText == 1)
		texture = vec3(1.0, 1.0, 0.0);
	else
		texture = vec3(1.0, 0.0, 1.0);*/

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
		
	gl_FragColor = colorTable * vec4(texture, 1.0);

}