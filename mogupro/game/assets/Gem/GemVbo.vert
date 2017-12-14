#version 150 core

uniform mat4	ciModelViewProjection;
uniform float   deltaTime;
uniform float   visibleRange;
uniform vec3   playerPos;

in vec4			ciPosition;
in vec4        ciColor;
in vec4        ciNormal;
in vec4        ciTexCoord0;
out vec4         vColor;		

void main( void ) {
	float dis;
	dis = distance(playerPos,ciPosition.xyz);
	gl_Position	= ciModelViewProjection * ciPosition;
	vec4 color = ciColor;
    color.a = sin((color.a + deltaTime) * 3.14159265 * 2 ) * 0.25 + 0.75;
	color.a =  color.a * min(float(visibleRange / dis), 1.0f);
    vColor = color;
}