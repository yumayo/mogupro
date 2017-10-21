#version 150 core

uniform vec4		uColor;
uniform sampler2D	uTex0;
		
in vec2				TexCoord0;
out vec4			oColor;

uniform vec2 uWindowSize;
		
void main( void ) {
	float bias = 1.0 / 49.0;
	vec4 calcColor = vec4(0, 0, 0, 0);
	vec2 m = vec2(1,1) / uWindowSize;

	for(int x = -3; x < 4; ++x)
	{
		for(int y = -3; y < 4; ++y)
		{
			vec2 pos = TexCoord0 + vec2(m.x * x, m.y * y);
			pos.y = 1 - pos.y;
			calcColor += texture( uTex0, pos ) * uColor * bias;
		}
	}
	//calcColor += texture( uTex0, TexCoord0 + vec2(-m.x * 2, -m.y * 2) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(-m.x, -m.y) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(0, -m.y) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(m.x, -m.y) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(-m.x, -m.y) ) * uColor * bias;
	//
	//calcColor += texture( uTex0, TexCoord0 + vec2(-m.x, 0) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(0, 0) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(m.x, 0) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(-m.x, m.y) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(0, m.y) ) * uColor * bias;
	//calcColor += texture( uTex0, TexCoord0 + vec2(m.x, m.y) ) * uColor * bias;
	oColor = calcColor;
}