#version 150 core

uniform sampler2D uTex0;
uniform vec4 uAmb;
uniform vec4 uModelViewLightPositions[100];
uniform vec4 uModelViewLightColors[100];
uniform int uLightNum;

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;
in vec4 vColor;

in vec4 vModelViewPosition;
in vec3 vModelViewNormal;

out vec4 oColor;

void main()
{
    vec4 col = texture( uTex0, vTexCoord0 );
    // 真っ黒のテクスチャが使えなくなっちゃうんですよね。
    if(col.rgb == vec3(0.0) )
    {
        oColor = vColor * uAmb;
    }
    else
    {
        oColor = texture( uTex0, vTexCoord0 ) * uAmb;
    }

    for(int i = 0; i < uLightNum; ++i)
    {
        float lightDistance = distance(uModelViewLightPositions[i].xyz, vModelViewPosition.xyz);
        if(lightDistance < 2.0)
        {
            oColor.rgb += uModelViewLightColors[i].rgb * (2.0 - lightDistance) * 0.5;
        }
    }
}