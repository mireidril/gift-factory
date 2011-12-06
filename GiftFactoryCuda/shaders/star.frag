#version 120

uniform mat4 model;
uniform mat4 view;

uniform float focalDistance;
uniform float focalRange;
varying float Blur;

void main (void)
{
	gl_FragColor = vec4( 0.0, 1.0, 1.0, 1.0);
}
