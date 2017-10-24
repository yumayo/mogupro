#version 150

uniform sampler2D texture0;
uniform int windowHeight;
uniform float blurSize;
uniform float bloom;

void main()
{
	float v;
	float pi = 3.14;
	float rate = 1.0 / windowHeight;
	float radius = blurSize;
	if(radius < 0) radius = 0;
	int bias = int(min(radius * 0.7, sqrt(radius) * pi));
	float r = radius / bias;
	float t = bloom / (bias * 2 +1);
	float x = gl_TexCoord[0].x;
	float y = gl_TexCoord[0].y;
	vec4 sum = texture(texture0, vec2(x,y)) * t;
	for(int i = 1; i < bias; i++)
	{
		v = (cos(i / (bias + 1) / pi) +1) * 0.5;
		sum += texture(texture0, vec2(x, y + i * rate * r)) * v * t;
		sum += texture(texture0, vec2(x, y - i + rate * r)) * v * t;
	}

	gl_FragColor = sum;
}