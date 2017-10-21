#version 150 core

uniform mat4 	ciModelView;
uniform mat3 	ciNormalMatrix;
uniform mat4 	ciModelViewProjection;

in vec4 ciColor;
in vec4 ciPosition;
in vec3 ciNormal;

out vec4 oColor;
out vec4 oPosition;
out vec3 oNormal;

void main()
{
	oColor       = ciColor;
	oPosition    = ciModelView * ciPosition;
	vec3 n		= ciNormal;
	oNormal      = ciNormalMatrix * n;

	gl_Position 	= ciModelViewProjection * ciPosition;
}