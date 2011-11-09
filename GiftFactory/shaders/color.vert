#version 120

void main(void)
{
	gl_Position = gl_ProjectionMatrix * gl_Vertex;
}
