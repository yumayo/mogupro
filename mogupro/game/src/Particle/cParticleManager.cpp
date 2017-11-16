#include <Particle/cParticleManager.h>
#include <Resource/TextureManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cLightManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Utility/cRandom.h>
#include "cinder/ip/Fill.h"
#include "cinder/ip/Blend.h"

using namespace ci;
using namespace ci::app;

namespace Particle
{
std::string getTextureNameFromTextureType( const ParticleTextureType& type )
{
    switch ( type )
    {
        case ParticleTextureType::NONE:
            return "";
        case ParticleTextureType::SAND:
            return "dirt";
        case ParticleTextureType::SPARK:
            return "spark";
    }
    return "";
}

float getLength( const vec3& p1, const vec3& p2 )
{
    vec3 vec = p2 - p1;
    return glm::sqrt( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z );
}

cParticle::cParticle( const ci::vec3& vec,
                      const ci::vec3& position,
                      const float& time ) :
    mPosition( position )
    , mVec( vec )
    , mTime( time )
{
}

void cParticle::update( const float& delta_time )
{
    mPosition += mVec;
    mTime -= delta_time;
}

bool cParticle::isActive()
{
    return mTime > 0;
}

cParticleHolder::cParticleHolder( const vec3& position,
                                  const ParticleType& type,
                                  const ParticleTextureType& texture_type,
                                  const float& time,
                                  const int& count ) :
    mPosition( position )
    , mType( type )
    , mTime( time )
{
    mTextureName = getTextureNameFromTextureType( texture_type );

    gl::Texture2d::Format fmt;
    fmt.enableMipmapping( true );
    fmt.setMinFilter( GL_LINEAR_MIPMAP_LINEAR );
    mTexture = gl::Texture::create( loadImage( loadAsset( mTextureName + ".png" ) ), fmt );

    gl::enableDepthWrite();
    gl::enableDepthRead();

    TEX->set( mTextureName, mTextureName + ".png" );

    mHandle = Game::cLightManager::getInstance()->addPointLight( mPosition,
                                                                 vec3( 1, 1, 1 ),
                                                                 2.0f );

    if ( mType == ParticleType::EXPROTION )
    {
        for ( size_t i = 0; i < count; i++ )
        {
            create( vec3( 0 ), time );
        }
    }
}

cParticleHolder::~cParticleHolder()
{
    Game::cLightManager::getInstance()->removePointLight( mHandle );
    mParticles.clear();
}

void cParticleHolder::update( const float& delta_time )
{
    sortByCamera();
    mTime -= delta_time;

    if ( mType != ParticleType::EXPROTION )
        create( vec3( 0 ), 3.0f );

    for ( auto& it = mParticles.begin(); it != mParticles.end(); )
    {
        ( *it )->update( delta_time );
        if ( ( *it )->isActive() == false )
            it = mParticles.erase( it );
        else
            it++;
    }
}

void cParticleHolder::draw( const glm::quat& rotation )
{
    using namespace ci::gl;
    auto ctx = context();

    gl::pushModelView();
    auto texture = TEX->get( mTextureName );
    if ( !texture )
        return;

    Rectf rect( vec2( -0.5, 1.0 ), vec2( 0.5, 0.0 ) );

    //gl::GlslProgRef mShader = ctx->getStockShader( gl::ShaderDef().color().texture() );;

    gl::color( Color( 1.0f, 0.5f, 0.25f ) );

    //ScopedGlslProg shader( mShader );
    ScopedTextureBind tex( mTexture );
    ScopedBlendAlpha blend();

    gl::color( Color( 1, 1, 1 ) );

    gl::translate( mPosition );
    for ( const auto& it : mParticles )
    {
        gl::pushModelView();
        gl::translate( it->mPosition );

        glm::fmat4 mat = glm::toMat4( rotation );
        gl::multModelMatrix( mat );

        gl::drawSolidRect( rect );
        //Rectf drawRect( 0, 0, mTexture->getWidth() / 3,
        //                mTexture->getHeight() / 3 );
        //gl::draw( mTexture, drawRect );
        //gl::drawSphere( vec3( 0 ), 1.0f, 60 );
        //gl::draw( mTexture, Area( rect ), rect );

        gl::popModelView();
    }
    gl::popModelView();

}

bool cParticleHolder::isActive()
{
    if ( mType == ParticleType::EXPROTION )
        return mParticles.size() >= 0;
    return mTime > 0;
}

void cParticleHolder::sortByCamera()
{
    vec3 camera_pos = CameraManager::cCameraManager::getInstance()->getCamera().getEyePoint();
    for ( size_t i = 0; i < mParticles.size(); i++ )
    {
        for ( size_t k = 0; k < mParticles.size() - i - 1; k++ )
        {
            float length1 = getLength( camera_pos, mParticles[k]->mPosition );
            float length2 = getLength( camera_pos, mParticles[k + 1]->mPosition );

            if ( length1 > length2 )
                std::swap( mParticles[k], mParticles[k + 1] );
        }
    }
}

void cParticleHolder::create( const ci::vec3& position,
                              const float& time )
{
    float range = 0.1f;
    Utility::RandomFloat r( -range, range );
    vec3 rand_vec = vec3( r(), r(), r() );
    mParticles.push_back( std::make_shared<cParticle>( rand_vec, position, time ) );
}

cParticleManager::cParticleManager()
{
}
cParticleManager::~cParticleManager()
{
}

void cParticleManager::setup()
{
    gl::enableAlphaBlending( true );
}

void cParticleManager::update( const float& delta_time )
{
    builbordUpdate();
    for ( auto& it = mParticleHolders.begin(); it != mParticleHolders.end(); )
    {
        ( *it )->update( delta_time );
        if ( ( *it )->isActive() == false )
            it = mParticleHolders.erase( it );
        else
            it++;
    }
}

void cParticleManager::draw()
{
	gl::ScopedGlslProg glsl( gl::getStockShader( gl::ShaderDef( ).texture( ) ) );
    for ( auto& it : mParticleHolders )
        it->draw( mBuilbordRotate );
}

void cParticleManager::create( const ci::vec3& position,
                               const ParticleType& type,
                               const ParticleTextureType& texture_type,
                               const float& time )
{
    mParticleHolders.push_back(
        std::make_shared<cParticleHolder>( position, type, texture_type, time, 0 ) );
}

void cParticleManager::create( const ci::vec3 & position,
                               const ParticleType & type,
                               const ParticleTextureType & texture_type,
                               const float& time,
                               const int & count )
{
    mParticleHolders.push_back(
        std::make_shared<cParticleHolder>( position, type, texture_type, time, count ) );
}

void cParticleManager::builbordUpdate()
{
    vec3 up_vec;
    vec3 right_vec;
    auto camera = CameraManager::cCameraManager::getInstance()->getCamera();
    camera.getBillboardVectors( &right_vec, &up_vec );

    vec3 toword_vec = glm::cross( right_vec, up_vec );
    toword_vec = glm::normalize( toword_vec );

    mBuilbordRotate = glm::quat( vec3( 0, 0, 1 ), toword_vec );
}
}
