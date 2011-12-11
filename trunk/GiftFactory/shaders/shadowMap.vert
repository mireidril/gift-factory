uniform sampler2D lightPointOfViewDepthBuffer;

uniform float lightRotation;
uniform vec3 lightPosition;
uniform vec4 materialDiffuse;

uniform int mode;
uniform mat4 lightViewMatrix;
uniform mat4 cameraViewMatrix;
uniform mat3 cameraNormalMatrix;
uniform mat4 lightRotationMatrix;

uniform vec4 lightPositionInScene;

attribute vec3 vert_input_position;
attribute vec2 vert_input_texcoord;
attribute vec3 vert_input_normal;

varying vec3 surfaceNormal;
varying vec3 lightToSurfaceVector;
varying vec4 fragmentPositionInCameraSpace;
varying vec4 fragmentPositionInLightSpace;
varying float NdotL;
varying vec4 lightPosCam;

void main(void)
{
	// ------------- Variables de sortie preexistantes ---------------
	
	// Position / Coordonnees de texture
	gl_TexCoord[0] = vec4( vert_input_texcoord, 0.0, 1.0 );
	
	// ----- Mode rendu depuis la lumiere -----
	if( mode == 0 ){
		
		// Position vertex dans l'espace camera
		fragmentPositionInCameraSpace = lightViewMatrix * ( gl_ModelViewMatrix * vec4( vert_input_position, 1.0 ) );
		fragmentPositionInCameraSpace /= fragmentPositionInCameraSpace.w;
	
		// Position projetee
		gl_Position = gl_ProjectionMatrix * fragmentPositionInCameraSpace;
		
		return;
	}
	
	// ----- Mode rendu depuis l'observateur -----
		
	// Position vertex dans l'espace camera
  	// On fait la meme chose que "gl_ModelViewMatrix * vert_input_position"...
  	// ...mais ici la gl_ModelViewMatrix en vue camera du rendu final est separee en cameraViewMatrix * gl_ModelViewMatrix
	fragmentPositionInCameraSpace = cameraViewMatrix * ( gl_ModelViewMatrix * vec4( vert_input_position, 1.0 ) );
	fragmentPositionInCameraSpace /= fragmentPositionInCameraSpace.w;
	
	// Position projetee
	gl_Position = gl_ProjectionMatrix * fragmentPositionInCameraSpace;
	
	// ------------- Variables varying a interpoler ---------------
	
	// Position du fragment dans le repere de la lumiere
  	// On fait la meme chose que "gl_ModelViewMatrix * vert_input_position"...
  	// ...mais ici la gl_ModelViewMatrix en vue lumiere est separee en lightViewMatrix * gl_ModelViewMatrix
	fragmentPositionInLightSpace = lightViewMatrix * ( gl_ModelViewMatrix * vec4( vert_input_position, 1.0 ) );
	fragmentPositionInLightSpace /= fragmentPositionInLightSpace.w;
	
  	// Vecteur normal dans le repere camera
  	// On fait la meme chose que "gl_NormalMatrix * vert_input_normal"...
  	// ...mais ici nous avons separe notre gl_NormalMatrix en deux sous-matrices cameraNormalMatrix * gl_NormalMatrix
	surfaceNormal = cameraNormalMatrix * ( gl_NormalMatrix * vert_input_normal );
	
	// Position lumiere dans l'espace camera
	vec4 lightPosCamSpace = cameraViewMatrix * ( lightRotationMatrix * vec4( lightPosition, 1.0 ) );
	lightPosCamSpace /= lightPosCamSpace.w;
	
	// Vecteur lumiere->surface dans l'espace camera
	lightToSurfaceVector = normalize( fragmentPositionInCameraSpace - lightPosCamSpace ).xyz;
}

