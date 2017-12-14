#version 150 core

uniform mat4	ciModelViewProjection;
uniform float   deltaTime;
in vec4			ciPosition;
in vec4        ciColor;
in vec4        ciNormal;
in vec4        ciTexCoord0;
out vec4         vColor;		

void main( void ) {
	gl_Position	= ciModelViewProjection * ciPosition;
	vec4 color = ciColor;
    color.a = sin((color.a + deltaTime) * 3.14159265 * 2 ) * 0.25 + 0.75;
    vColor = color;
}