#version 130

varying vec3 posVertex;
varying vec2 texCoords;
varying vec3 normals;

uniform mat4 model;
uniform mat4 view;

uniform float focalDistance;
uniform float focalRange;

varying float Blur;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * view * model * gl_Vertex;
	
	//posVertex = vec3( (gl_ProjectionMatrix * view * model * gl_Vertex).xyz );
	//texCoords = gl_MultiTexCoord0.xy;
	//normals = gl_NormalMatrix * gl_Normal;
	
	posVertex = gl_Vertex.xyz;
 	texCoords = gl_TexCoord[0].xy;
	normals =  gl_Normal;

	vec4 PosWV = gl_ModelViewMatrix * gl_Vertex;
	PosWV /= PosWV.w;
	Blur = clamp(abs(-PosWV.z - focalDistance) / focalRange, 0.0, 1.0);
	//Blur = 0.5;
}

