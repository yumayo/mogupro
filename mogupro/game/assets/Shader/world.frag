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
uniform bool uUseShadow;
uniform vec3 uShadowCameraViewPos;
uniform sampler2DShadow uShadowMap;
in vec4 vShadowCoord;
in vec4 vShadowPosition;
in vec3 vShadowNormal;

// ポイントライトに使うやつ
layout (std140) uniform PointLightParams
{
    vec4 useIndices[384 / 4];
    vec4 modelViewPositions[384];
    vec4 colorWithRadiuses[384];
}uPointLight;
uniform int uPointLightNum;

// ラインライトに使うやつ
layout (std140) uniform LineLightParams
{
    vec4 useIndices[64 / 4];
    vec4 modelViewPositionsA[64];
    vec4 modelViewPositionsB[64];
    vec4 colorWithRadiuses[64];
}uLineLight;
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
    if(uUseShadow)
    {
        oColor.rgb *= ( Diffuse * Shadow + Ambient );
    }

    // ポイントライト
    for(int i = 0; i < uPointLightNum; ++i)
    {
        int useId = int( uPointLight.useIndices[i / 4][i % 4] );
        float d = distance( vModelViewPosition.xyz, uPointLight.modelViewPositions[useId].xyz );
        if(d < uPointLight.colorWithRadiuses[useId].a)
        {
            oColor.rgb += uPointLight.colorWithRadiuses[useId].xyz * (uPointLight.colorWithRadiuses[useId].a - d) * ( 1.0 / uPointLight.colorWithRadiuses[useId].a );
        }
    }

    // ラインライト
    for(int i = 0; i < uLineLightNum; ++i)
    {
        int useId = int( uLineLight.useIndices[i / 4][i % 4] );
        float d = distancePointLine( vModelViewPosition.xyz, uLineLight.modelViewPositionsA[useId].xyz, uLineLight.modelViewPositionsB[useId].xyz );
        if(d < uLineLight.colorWithRadiuses[useId].a)
        {
            oColor.rgb += uLineLight.colorWithRadiuses[useId].xyz * (uLineLight.colorWithRadiuses[useId].a - d) * ( 1.0 / uLineLight.colorWithRadiuses[useId].a );
        }
    }
}