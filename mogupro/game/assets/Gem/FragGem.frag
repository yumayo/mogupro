#version 150 core

uniform vec4		uColor;
uniform sampler2D	uTex0;


in vec2				TexCoord0;
out vec4			oColor;

uniform vec2 uWindowSize;

void main()
{
	float bias = 1.0 / 40;
	vec4 calcColor = vec4(0, 0, 0, 0);
	vec2 m = vec2(1,1) / (uWindowSize);

	int x = 0;
	int y = 0;

	for(y = 0; y < 40; ++y)
	{
		vec2 pos = TexCoord0 + vec2(m.x * y, m.y * y);
		pos.y = 1 - pos.y;
		calcColor += texture( uTex0, pos ) * uColor * bias;
	    pos = TexCoord0 + vec2(m.x * y, -m.y * y);
		pos.y = 1 - pos.y;
		calcColor += texture( uTex0, pos ) * uColor * bias;
		pos = TexCoord0 + vec2(-m.x * y, m.y * y);
		pos.y = 1 - pos.y;
		calcColor += texture( uTex0, pos ) * uColor * bias;
		pos = TexCoord0 + vec2(-m.x * y, -m.y * y);
		pos.y = 1 - pos.y;
		calcColor += texture( uTex0, pos ) * uColor * bias;
	}

	calcColor.a = calcColor.a;
	oColor = calcColor;
}