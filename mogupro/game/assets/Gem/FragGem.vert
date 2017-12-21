#version 150 core

uniform mat4	ciModelViewProjection;
in vec4			ciPosition;
in vec2			ciTexCoord0;
in vec3         ciColor;
out vec4        oPosition;
out vec2		TexCoord0;
out vec3        oColor;		


void main( void ) {
	oPosition = ciPosition;
	gl_Position	= ciModelViewProjection * ciPosition;
	TexCoord0 = ciTexCoord0;
	oColor = ciColor;
}