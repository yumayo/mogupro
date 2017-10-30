#version 150 core

uniform vec4		uColor;
//uniform float       uAlpha;
uniform sampler2D	uTex0;

in vec2				TexCoord0;
out vec4			oColor;

uniform vec2 uWindowSize;
		
void main( void ) {
	float bias = 1.0 / 120;
	vec4 calcColor = vec4(0, 0, 0, 0);
	vec2 m = vec2(1,1) / uWindowSize;


	for(int x = -10; x < 10; ++x)
	{
		for(int y = -10; y < 10; ++y)
		{
			vec2 pos = TexCoord0 + vec2(m.x * x, m.y * y);
			pos.y = 1 - pos.y;
			calcColor += texture( uTex0, pos ) * uColor * bias;
		}
	}

	oColor = calcColor;
}