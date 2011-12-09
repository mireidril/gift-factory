#version 120

uniform mat4 model;
uniform mat4 view;

uniform float focalDistance;
uniform float focalRange;
varying float Blur;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * view * model * gl_Vertex;

	vec4 PosWV = gl_ModelViewMatrix * gl_Vertex;
	PosWV /= PosWV.w;
	Blur = clamp(abs(-PosWV.z - focalDistance) / focalRange, 0.0, 1.0);
	//Blur = 0.5;
}

