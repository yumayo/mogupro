#version 150

uniform sampler2D uTex0;

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;
in vec4 vColor;

void main()
{
    oColor += texture( uTex0, vTexCoord0 );
}