#version 150 core

uniform mat4	ciModelViewProjection;
in vec4			ciPosition;
in vec4        ciColor;
out vec4         vColor;		

void main( void ) {

	gl_Position	= ciModelViewProjection * ciPosition;
	vColor = ciColor;
}