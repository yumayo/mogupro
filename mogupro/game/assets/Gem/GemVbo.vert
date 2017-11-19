#version 150 core

uniform mat4	ciModelViewProjection;
uniform float   deltaTime;
in vec4			ciPosition;
in vec4        ciColor;
out vec4         vColor;		

void main( void ) {
	float pai = 3.14;
	gl_Position	= ciModelViewProjection * ciPosition;
	vec4 color = ciColor;
    color.a = abs(sin((color.a + deltaTime) * 3.14 )) * 0.5 + 0.3;
    vColor = color;
}