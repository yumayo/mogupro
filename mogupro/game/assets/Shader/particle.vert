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

void main()
{
    vPosition = ciPosition;
	vNormal = normalize( ciNormal );
	vTexCoord0 = ciTexCoord0;
	vColor = ciColor;

    gl_Position	= ciModelViewProjection * ciPosition;
}