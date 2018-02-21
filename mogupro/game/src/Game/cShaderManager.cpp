#include <Game/cShaderManager.h>
#include <cinder/gl/gl.h>
#include <Game/cGemManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cLightManager.h>
#include <Game/cDebugManager.h>
using namespace ci;
namespace Game
{
void cShaderManager::setup( )
{
	mUseShadow = false;
	mUseAllLight = false;

	int scale = 4096;
	gl::Texture2d::Format depthFormat;
	depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
	depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
	depthFormat.setMagFilter( GL_LINEAR );
	depthFormat.setMinFilter( GL_LINEAR );
	depthFormat.setWrap( GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER );
	depthFormat.setBorderColor( ColorA( 1, 1, 1, 1 ) );
	depthFormat.setCompareFunc( GL_LEQUAL );
	mShadowTex = gl::Texture2d::create( scale, scale, depthFormat );

	gl::Fbo::Format fboFormat;
	fboFormat.attachment( GL_DEPTH_ATTACHMENT, mShadowTex );
	mFbo = gl::Fbo::create( mShadowTex->getWidth( ), mShadowTex->getHeight( ), fboFormat );

	mGlsl = gl::GlslProg::create( app::loadAsset( "Shader/world.vert" ),
									app::loadAsset( "Shader/world.frag" ) );

	int size = 128;
	vec3 pos( 80, 80, 80 );
	mCamera.setOrtho( -size, size, -size, size, 0.25F, 512.0F );
	mCamera.lookAt( pos + vec3( size, size, -size ), pos + vec3( 0, 0, 0 ) );

	mPointLightUBO = gl::Ubo::create( sizeof( mPointLightParams ), &mPointLightParams, GL_DYNAMIC_DRAW );
	mLineLightUBO = gl::Ubo::create( sizeof( mLineLightParams ), &mLineLightParams, GL_DYNAMIC_DRAW );
	mSpotLightUBO = gl::Ubo::create( sizeof( mSpotLightParams ), &mSpotLightParams, GL_DYNAMIC_DRAW );

	cDebugManager::getInstance( )->mParam->addParam( "UseShadow", &mUseShadow );
	cDebugManager::getInstance( )->mParam->addParam( "UseAllLight", &mUseAllLight );
}
void cShaderManager::uniformUpdate( )
{
	if ( !mUseAllLight )
	{
		mGlsl->uniform( "uPointLightNum", 0 );
		mGlsl->uniform( "uLineLightNum", 0 );
		mGlsl->uniform( "uSpotLightNum", 0 );
	}
	else
	{
		{
			auto lights = Game::cLightManager::getInstance( )->getPointLights( );
			memset( &mPointLightParams, 0, sizeof( mPointLightParams.useIndices ) );
			int i = 0;
			for ( auto& light : lights )
			{
				if ( !light.second )
				{
					continue;
				}
				else
				{
					mPointLightParams.useIndices[i / 4][i % 4] = i;
					i++;
				}
			}
			mGlsl->uniform( "uPointLightNum", i );
			mPointLightUBO->bufferSubData( 0, sizeof( mPointLightParams.useIndices ), &mPointLightParams );
			mGlsl->uniformBlock( "PointLightParams", 0 );
		}
		{
			auto lights = Game::cLightManager::getInstance( )->getLineLights( );
			memset( &mLineLightParams, 0, sizeof( mLineLightParams.useIndices ) );
			int i = 0;
			for ( auto& light : lights )
			{
				if ( !light.second )
				{
					continue;
				}
				else
				{
					mLineLightParams.useIndices[i / 4][i % 4] = i;
					i++;
				}
			}
			mGlsl->uniform( "uLineLightNum", i );
			mLineLightUBO->bufferSubData( 0, sizeof( mLineLightParams.useIndices ), &mLineLightParams );
			mGlsl->uniformBlock( "LineLightParams", 1 );
		}
		{
			auto lights = Game::cLightManager::getInstance( )->getSpotLights( );
			memset( &mSpotLightParams, 0, sizeof( mSpotLightParams.useIndices ) );
			int i = 0;
			for ( auto& light : lights )
			{
				if ( !light.second )
				{
					continue;
				}
				else
				{
					mSpotLightParams.useIndices[i / 4][i % 4] = i;
					i++;
				}
			}
			mGlsl->uniform( "uSpotLightNum", i );
			mSpotLightUBO->bufferSubData( 0, sizeof( mSpotLightParams.useIndices ), &mSpotLightParams );
			mGlsl->uniformBlock( "SpotLightParams", 2 );
		}
	}
}
void cShaderManager::uniformUpdate( int chunkId )
{
	if ( auto lights = Game::cLightManager::getInstance( )->getPointLights( chunkId ) )
	{
		memset( &mPointLightParams, 0, sizeof( mPointLightParams.useIndices ) );
		int i = 0;
		for ( auto& light : *lights )
		{
			if ( !light )
			{
				continue;
			}
			else
			{
				mPointLightParams.useIndices[i / 4][i % 4] = light->getId( );
				i++;
			}
		}
		mGlsl->uniform( "uPointLightNum", i );
		mPointLightUBO->bufferSubData( 0, sizeof( mPointLightParams.useIndices ), &mPointLightParams );
		mGlsl->uniformBlock( "PointLightParams", 0 );
	}
	if ( auto lights = Game::cLightManager::getInstance( )->getLineLights( chunkId ) )
	{
		memset( &mLineLightParams, 0, sizeof( mLineLightParams.useIndices ) );
		int i = 0;
		for ( auto& light : *lights )
		{
			if ( !light )
			{
				continue;
			}
			else
			{
				mLineLightParams.useIndices[i / 4][i % 4] = light->getId( );
				i++;
			}
		}
		mGlsl->uniform( "uLineLightNum", i );
		mLineLightUBO->bufferSubData( 0, sizeof( mLineLightParams.useIndices ), &mLineLightParams );
		mGlsl->uniformBlock( "LineLightParams", 1 );
	}
	if ( auto lights = Game::cLightManager::getInstance( )->getSpotLights( chunkId ) )
	{
		memset( &mSpotLightParams, 0, sizeof( mSpotLightParams.useIndices ) );
		int i = 0;
		for ( auto& light : *lights )
		{
			if ( !light )
			{
				continue;
			}
			else
			{
				mSpotLightParams.useIndices[i / 4][i % 4] = light->getId( );
				i++;
			}
		}
		mGlsl->uniform( "uSpotLightNum", i );
		mSpotLightUBO->bufferSubData( 0, sizeof( mSpotLightParams.useIndices ), &mSpotLightParams );
		mGlsl->uniformBlock( "SpotLightParams", 2 );
	}
}
void cShaderManager::uniformUpdate(std::vector<int> const & pointLightIds, std::vector<int> const & lineLightIds, std::vector<int> const & spotLightIds)
{
	{
		memset(&mPointLightParams, 0, sizeof(mPointLightParams.useIndices));
		int i = 0;
		for (auto& id : pointLightIds)
		{
			mPointLightParams.useIndices[i / 4][i % 4] = id;
			i++;
		}
		mGlsl->uniform("uPointLightNum", i);
		mPointLightUBO->bufferSubData(0, sizeof(mPointLightParams.useIndices), &mPointLightParams);
		mGlsl->uniformBlock("PointLightParams", 0);
	}
	{
		memset(&mLineLightParams, 0, sizeof(mLineLightParams.useIndices));
		int i = 0;
		for (auto& id : lineLightIds)
		{
			mLineLightParams.useIndices[i / 4][i % 4] = id;
			i++;
		}
		mGlsl->uniform("uLineLightNum", i);
		mLineLightUBO->bufferSubData(0, sizeof(mLineLightParams.useIndices), &mLineLightParams);
		mGlsl->uniformBlock("LineLightParams", 1);
	}
	{
		memset(&mSpotLightParams, 0, sizeof(mSpotLightParams.useIndices));
		int i = 0;
		for (auto& id : spotLightIds)
		{
			mSpotLightParams.useIndices[i / 4][i % 4] = id;
			i++;
		}
		mGlsl->uniform("uSpotLightNum", i);
		mSpotLightUBO->bufferSubData(0, sizeof(mSpotLightParams.useIndices), &mSpotLightParams);
		mGlsl->uniformBlock("SpotLightParams", 2);
	}
}
void cShaderManager::update( std::function<void( )> const& drawFunc )
{
	if ( mUseShadow )
	{
		gl::enable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 2.0f, 2.0f );
		gl::ScopedFramebuffer framebuffer( mFbo );
		gl::ScopedViewport viewport( mFbo->getSize( ) );
		gl::ScopedGlslProg glsl( cinder::gl::getStockShader( cinder::gl::ShaderDef( ).texture( ) ) );
		gl::clear( );
		gl::enableDepth( );
		gl::enableAlphaBlending( );
		gl::setMatrices( mCamera );
		drawFunc( );
		gl::disable( GL_POLYGON_OFFSET_FILL );
	}
}
void cShaderManager::draw( std::function<void( )> const& render )
{
	gl::ScopedGlslProg scpGlsl( mGlsl );

	mPointLightUBO->bindBufferBase( 0 );
	mLineLightUBO->bindBufferBase( 1 );
	mSpotLightUBO->bindBufferBase( 2 );

	mGlsl->uniform( "uAmb", vec4( 99 / 255.0F, 161 / 255.0F, 255 / 255.0F, 1.0F ) );

	{
		// �|�C���g���C�g�̃f�[�^�𑗂�܂��B
		auto lights = Game::cLightManager::getInstance( )->getPointLights( );
		memset( &mPointLightParams, 0, sizeof( mPointLightParams ) );
		{
			for ( auto& light : lights )
			{
				if ( !light.second )
				{
					continue;
				}
				else
				{
					int id = light.second->getId( );
					mPointLightParams.modelViewPositions[id] = CAMERA->getCamera( ).getViewMatrix( ) * vec4( light.second->getPosition( ), 1 );
					mPointLightParams.colorWithRadiuses[id].x = light.second->color.x;
					mPointLightParams.colorWithRadiuses[id].y = light.second->color.y;
					mPointLightParams.colorWithRadiuses[id].z = light.second->color.z;
					mPointLightParams.colorWithRadiuses[id].w = light.second->getRadius( );
				}
			}
			mGlsl->uniform( "uPointLightNum", 0 );
			mPointLightUBO->bufferSubData( 0, sizeof( mPointLightParams ), &mPointLightParams );
			mGlsl->uniformBlock( "PointLightParams", 0 );
		}
	}
	{
		// ���C�����C�g�̏��𑗂�܂��B
		auto lights = Game::cLightManager::getInstance( )->getLineLights( );
		memset( &mLineLightParams, 0, sizeof( mLineLightParams ) );
		{
			for ( auto& light : lights )
			{
				if ( !light.second )
				{
					continue;
				}
				else
				{
					int id = light.second->getId( );
					mLineLightParams.modelViewPositionsA[id] = CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light.second->getBeginPosition( ), 1 );
					mLineLightParams.modelViewPositionsB[id] = CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light.second->getEndPosition( ), 1 );
					mLineLightParams.colorWithRadiuses[id].x = light.second->color.x;
					mLineLightParams.colorWithRadiuses[id].y = light.second->color.y;
					mLineLightParams.colorWithRadiuses[id].z = light.second->color.z;
					mLineLightParams.colorWithRadiuses[id].a = light.second->getRadius( );
				}
			}
			mGlsl->uniform( "uLineLightNum", 0 );
			mLineLightUBO->bufferSubData( 0, sizeof( mLineLightParams ), &mLineLightParams );
			mGlsl->uniformBlock( "LineLightParams", 1 );
		}
	}
	{
		// �X�|�b�g���C�g�̏��𑗂�܂��B
		auto lights = Game::cLightManager::getInstance( )->getSpotLights( );
		memset( &mSpotLightParams, 0, sizeof( mSpotLightParams ) );
		{
			for ( auto& light : lights )
			{
				if ( !light.second )
				{
					continue;
				}
				else
				{
					int id = light.second->getId( );
					mSpotLightParams.modelViewPositionsA[id] = CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light.second->getPosition( ), 1 );
					mSpotLightParams.modelViewPositionsB[id] = CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light.second->getPosition( ) + light.second->getDirection( ), 1 );
					mSpotLightParams.colorWithRadiuses[id].x = light.second->color.x;
					mSpotLightParams.colorWithRadiuses[id].y = light.second->color.y;
					mSpotLightParams.colorWithRadiuses[id].z = light.second->color.z;
					mSpotLightParams.colorWithRadiuses[id].a = light.second->getRadius( );
				}
			}
			mGlsl->uniform( "uSpotLightNum", 0 );
			mSpotLightUBO->bufferSubData( 0, sizeof( mSpotLightParams ), &mSpotLightParams );
			mGlsl->uniformBlock( "SpotLightParams", 2 );
		}
	}

	gl::ScopedTextureBind texScope( mShadowTex, (uint8_t)1 );
	vec3 shadowCameraViewPos = vec3( CAMERA->getCamera( ).getViewMatrix( ) * vec4( mCamera.getEyePoint( ), 1.0f ) );
	mat4 shadowView = mCamera.getProjectionMatrix( ) * mCamera.getViewMatrix( );
	mGlsl->uniform( "uShadowMap", 1 );
	mGlsl->uniform( "uShadowCameraViewPos", shadowCameraViewPos );
	mGlsl->uniform( "uShadowView", shadowView );
	mGlsl->uniform( "uUseShadow", mUseShadow );

	render( );
}
}
