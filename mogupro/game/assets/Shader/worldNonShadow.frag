#version 150 core

uniform sampler2D uTex0;
uniform vec4 uAmb;

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;
in vec4 vColor;

in vec4 vModelViewPosition;
in vec3 vModelViewNormal;

out vec4 oColor;

// ポイントライトに使うやつ
uniform vec3 uModelViewPointLightPositions[100];
uniform vec3 uModelViewPointLightColors[100];
uniform float uModelViewPointLightRadiuses[100];
uniform int uPointLineNum;

// ラインライトに使うやつ
uniform vec3 uModelViewLineLightPositionsA[100];
uniform vec3 uModelViewLineLightPositionsB[100];
uniform vec3 uModelViewLineLightColors[100];
uniform float uModelViewLineLightRadiuses[100];
uniform int uLineLightNum;

float distancePointLine( vec3 P, vec3 A, vec3 B )
{
    vec3 a = B - A;
    vec3 b = P - A;
    float t = clamp(dot(a, b) / dot(a, a), 0.0, 1.0);
    return distance(P, A + a * t);
}

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
        oColor = col * uAmb;
    }

    // ポイントライト
    for(int i = 0; i < uPointLineNum; ++i)
    {
        float lightDistance = distance( vModelViewPosition.xyz, uModelViewPointLightPositions[i] );
        if(lightDistance < uModelViewPointLightRadiuses[i])
        {
            oColor.rgb += uModelViewPointLightColors[i] * (uModelViewPointLightRadiuses[i] - lightDistance) * ( 1.0 / uModelViewPointLightRadiuses[i] );
        }
    }

    // ラインライト
    for(int i = 0; i < uLineLightNum; ++i)
    {
        float d = distancePointLine(vModelViewPosition.xyz, uModelViewLineLightPositionsA[i], uModelViewLineLightPositionsB[i] );
        if(d < uModelViewLineLightRadiuses[i])
        {
            oColor.rgb += uModelViewLineLightColors[i] * (uModelViewLineLightRadiuses[i] - d) * ( 1.0 / uModelViewLineLightRadiuses[i] );
        }
    }
}