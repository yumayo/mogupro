#version 150

uniform sampler2D uTex0;
uniform vec4 uAmb;

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;
in vec4 vColor;

in vec4 vModelViewPosition;
in vec3 vModelViewNormal;

out vec4 oColor;

// 影に使うやつ
uniform vec3 uShadowCameraViewPos;
uniform sampler2DShadow uShadowMap;
in vec4 vShadowCoord;
in vec4 vShadowPosition;
in vec3 vShadowNormal;

// ポイントライトに使うやつ
layout (std140) uniform PointLightParams
{
    vec3 uPointLightModelViewPositions[100];
    vec3 uPointLightColors[100];
    float uPointLightRadiuses[100];
};
uniform int uPointLightNum;

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

    // 影
	vec3 LightVec		= normalize( uShadowCameraViewPos );
    float NdotL			= max( dot( vShadowNormal, LightVec ), 0.0 );
	vec3 Diffuse		= vec3( NdotL );
	vec3 Ambient		= vec3( 0.8 );
	float Shadow		= 1.0;
	vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;
	if ( ShadowCoord.z > -1 && ShadowCoord.z < 1 ) 
	{
		Shadow = textureProj( uShadowMap, ShadowCoord, -0.00005 );
	}
    oColor.rgb *= ( Diffuse * Shadow + Ambient );

    // ポイントライト
    for(int i = 0; i < uPointLightNum; ++i)
    {
        float lightDistance = distance( vModelViewPosition.xyz, uPointLightModelViewPositions[i] );
        if(lightDistance < 1)
        {
            oColor.rgb = uPointLightColors[i];
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