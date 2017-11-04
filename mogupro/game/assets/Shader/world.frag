#version 150 core
uniform sampler2D uTex0;
uniform vec4 uAmb;
in vec2 vTexCoord0;
in vec4 vColor;
out vec4 oColor;
void main()
{
    vec4 col = texture( uTex0, vTexCoord0 );
    if(col.rgb == vec3(0.0) )
    {
        oColor = vColor * uAmb;
    }
    else
    {
        oColor = texture( uTex0, vTexCoord0 ) * uAmb;
    }
}