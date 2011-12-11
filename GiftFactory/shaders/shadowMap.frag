uniform sampler2D lightPointOfViewDepthBuffer;

uniform float lightRotation;
uniform vec3 lightPosition;
uniform vec4 materialDiffuse;

uniform int mode;
uniform mat4 lightViewMatrix;
uniform mat4 cameraViewMatrix;
uniform mat3 cameraNormalMatrix;
uniform mat4 lightRotationMatrix;

varying vec3 surfaceNormal;
varying vec3 lightToSurfaceVector;
varying vec4 fragmentPositionInCameraSpace;
varying vec4 fragmentPositionInLightSpace;

void main(void)
{
	// ----- Cas de la 1ere passe en vue depuis la source -----
	if( mode == 0 ){
		
		// Ecriture d'une couleur arbitraire sans utilite
		// Car pour la vue depuis la source, seule la profondeur nous interesse
		gl_FragColor = fragmentPositionInCameraSpace / fragmentPositionInCameraSpace.w;
		
		// On quitte immediatement, cette passe etant terminee
		return;
	}
	
	// ----- Cas de la 2eme passe en vue traditionelle depuis l'observateur -----
	
	// Normalisation indispensable des vecteurs apres interpolation
	vec3 normalizedNormal = normalize( surfaceNormal );
	vec3 normalizedLightDir = normalize( lightToSurfaceVector );
	
	// Calcul de la position du fragment dans le repere camera projete de la vue depuis la source
	vec4 projectedOnSourceScreen = gl_ProjectionMatrix * fragmentPositionInLightSpace;
	projectedOnSourceScreen /= projectedOnSourceScreen.w;
	
	// ----------------------------------------------
	
	// La surface vue depuis l'observateur est-elle ombree ?
	// Oui si :
	// 1. Ce fragment vu depuis la camera est bel et bien dans le champ de vision de la vue depuis la source
	// 2. La profondeur du fragment en vue lumiere est superieure a la surface vue par la lumiere dans la premiere passe
	
	bool fragmentIsVisibleBySource = projectedOnSourceScreen.x >= -1.0 && projectedOnSourceScreen.x <= 1.0
	                              && projectedOnSourceScreen.y >= -1.0 && projectedOnSourceScreen.y <= 1.0;
	
	// Ou doit-on aller lire dans la shadow map ?
	// --> Nous avons projete le fragment courant sur la surface ecran en rendu depuis la lumiere
	// --> On ramene ces coordonnees (projetees entre -1 et 1) entre 0 et 1
	vec2 shadowMapTexCoords = ( projectedOnSourceScreen.xy + 1.0 ) / 2.0;
	float shadowFactor = 1.0;
	if( fragmentIsVisibleBySource ){
		if( fragmentPositionInLightSpace.z + 0.1 < texture2D( lightPointOfViewDepthBuffer, ( projectedOnSourceScreen.xy + 1.0 ) / 2.0 ).z ){
			shadowFactor = 0.0;
		}
	}
	
	// ----------------------------------------------
	
	// Ponderation de la lumiere arrivant sur la surface en fonction de l'inclinaison par rapport à la normale
	float lambertFactor = dot( normalizedNormal, -normalizedLightDir );
	
	// Ecriture de la couleur finale
	gl_FragColor = materialDiffuse * lambertFactor * shadowFactor;
}

