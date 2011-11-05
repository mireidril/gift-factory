attribute vec3 normals;

varying vec3 normal;

void main(void)
{
	normal = normals;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
