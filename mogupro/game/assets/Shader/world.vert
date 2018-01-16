#version 150

uniform mat4 ciModelView;
uniform mat4 ciModelMatrix;
uniform mat3 ciNormalMatrix;
uniform mat4 ciModelViewProjection;

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;
in vec4 ciColor;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord0;
out vec4 vColor;

out vec4 vModelViewPosition;
out vec3 vModelViewNormal;

// 影に使うやつ
uniform mat4 uShadowView;
out vec4 vShadowCoord;
const mat4 biasView = mat4( 0.5, 0.0, 0.0, 0.0,
							0.0, 0.5, 0.0, 0.0,
							0.0, 0.0, 0.5, 0.0,
							0.5, 0.5, 0.5, 1.0 );
out vec3 vShadowNormal;
out vec4 vShadowPosition;

void main( void ) 
{
	vPosition = ciPosition;
	vNormal = normalize( ciNormal );
	vTexCoord0 = ciTexCoord0;
	vColor = ciColor;

	vModelViewPosition = ciModelView * vPosition;
	vModelViewNormal = normalize( (ciModelView * vec4(vNormal, 0.0)).xyz );

	// 影の位置を計算します。
	vShadowCoord = ( biasView * uShadowView * ciModelMatrix ) * ciPosition;
	vShadowNormal = normalize( ciNormalMatrix * ciNormal );
	vShadowPosition = ciModelView * ciPosition;

	gl_Position	= ciModelViewProjection * ciPosition;
}