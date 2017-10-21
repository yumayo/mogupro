#version 150 core

uniform sampler2D texture;

out vec4 oColor;

void main( void )
{
	oColor = texture2DProj(texture, gl_TexCoord[0]);
}