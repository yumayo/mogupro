#version 150 core
uniform mat4 ciModelViewProjection;
in vec4 ciPosition;
in vec2 ciTexCoord0;
out vec2 vTexCoord0;
in vec4 ciColor;
out vec4 vColor;
void main( void ) 
{
	gl_Position	= ciModelViewProjection * ciPosition;
	vTexCoord0 = ciTexCoord0;
	vColor = ciColor;
}