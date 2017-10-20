#include <Shader/cShadowManager.h>
#include <cinder/app/App.h>
#include <cinder/gl/scoped.h>
#include <Scene/cSceneManager.h>
using namespace cinder;
namespace Shader
{
void cShadowManager::setup( )
{
    // 影を投影するテクスチャを作成。
    int scale = 8192;
    gl::Texture2d::Format depthFormat;
    depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
    depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
    depthFormat.setMagFilter( GL_LINEAR );
    depthFormat.setMinFilter( GL_LINEAR );
    depthFormat.setWrap( GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER );
    depthFormat.setBorderColor( ColorA( 1, 1, 1, 1 ) );
    depthFormat.setCompareFunc( GL_LEQUAL );
    mShadowTex = gl::Texture2d::create( scale, scale, depthFormat );

    // 上記で作成したテクスチャをフレームバッファオブジェクトにリンクします。
    gl::Fbo::Format fboFormat;
    fboFormat.attachment( GL_DEPTH_ATTACHMENT, mShadowTex );
    fbo = gl::Fbo::create( mShadowTex->getWidth( ), mShadowTex->getHeight( ), fboFormat );

    // 影用のシェーダーをコンパイルします。
    shader = gl::GlslProg::create( app::loadAsset( "Shader/simpleShadow.vert" ), app::loadAsset( "Shader/simpleShadow.frag" ) );

    // 影を投影するためのカメラを用意します。
    int size = 128;
    vec3 pos( 60, 60, 60 );
    mCamera.setOrtho( -size, size, -size, size, 0.25F, 512.0F );
    mCamera.lookAt( pos + vec3( size, size, -size ), pos + vec3( 0, 0, 0 ) );
}
void cShadowManager::update( std::function<void( )> const& drawFunc )
{
    gl::enable( GL_POLYGON_OFFSET_FILL );
    glPolygonOffset( 2.0f, 2.0f );
    gl::ScopedFramebuffer framebuffer( fbo );
    gl::ScopedViewport viewport( fbo->getSize( ) );
    gl::clear( );
    gl::enableDepth( );
    gl::enableAlphaBlending( );
    gl::setMatrices( mCamera );
    drawFunc( );
    gl::disable( GL_POLYGON_OFFSET_FILL );
}
void cShadowManager::draw( std::function<void( )> const& drawFunc )
{
    gl::ScopedGlslProg glslScp( shader );
    gl::ScopedTextureBind texScope( mShadowTex, (uint8_t)0 );

    vec3 mvLightPos = vec3( gl::getModelView( ) * vec4( mCamera.getEyePoint( ), 1.0f ) );
    mat4 shadowMatrix = mCamera.getProjectionMatrix( ) * mCamera.getViewMatrix( );

    shader->uniform( "uShadowMap", 0 );
    shader->uniform( "uLightPos", mvLightPos );
    shader->uniform( "uShadowMatrix", shadowMatrix );

    drawFunc( );
}
}
