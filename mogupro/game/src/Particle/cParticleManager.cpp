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

template<typename T>
void sortByCamera( std::vector<std::shared_ptr<T>> list, const vec3& offset_pos )
{
    vec3 camera_pos = CameraManager::cCameraManager::getInstance()->getCamera().getEyePoint();

    for ( size_t i = 0; i < list.size(); i++ )
    {
        for ( size_t k = 0; k < list.size() - i - 1; k++ )
        {
            const vec3& p1_pos = offset_pos + list[k]->mPosition;
            const vec3& p2_pos = offset_pos + list[k + 1]->mPosition;

            float length1 = getLength( camera_pos, p1_pos );
            float length2 = getLength( camera_pos, p2_pos );

            if ( length1 < length2 )
                std::swap( list[k], list[k + 1] );
        }
    }
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
                                  const int& count,
                                  const float& speed ) :
    mPosition( position )
    , mType( type )
    , mTime( time )
    , mSpeed( speed )
{
    mTextureName = getTextureNameFromTextureType( texture_type );
    TEX->set( mTextureName, mTextureName + ".png" );

    mHandle = Game::cLightManager::getInstance()->addPointLight( mPosition,
                                                                 vec3( 1, 1, 1 ),
                                                                 2.0f );

    if ( mType == ParticleType::EXPROTION )
        for ( int i = 0; i < count; i++ )
            create( vec3( 0 ), time );
}

cParticleHolder::~cParticleHolder()
{
    Game::cLightManager::getInstance()->removePointLight( mHandle );
    mParticles.clear();
}

void cParticleHolder::update( const float& delta_time )
{
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

    sort();
}

void cParticleHolder::draw( const glm::quat& rotation )
{
    using namespace ci::gl;
    auto ctx = context();

    gl::pushModelView();
    auto texture = TEX->get( mTextureName );
    if ( !texture )
        return;

    Rectf rect( vec2( -0.5, -0.5 ), vec2( 0.5, 0.5 ) );

    ScopedTextureBind tex( texture );
    ScopedBlendAlpha blend;

    gl::translate( mPosition );
    for ( const auto& it : mParticles )
    {
        ScopedColor color( 1, 1, 1, it->mTime );
        gl::pushModelView();
        gl::translate( it->mPosition );
        glm::fmat4 mat = glm::toMat4( rotation );
        gl::multModelMatrix( mat );

        gl::drawSolidRect( rect );

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

void cParticleHolder::sort()
{
    sortByCamera<cParticle>( mParticles, mPosition );
}

void cParticleHolder::create( const ci::vec3& position,
                              const float& time )
{
    Utility::RandomFloat rv( -mSpeed, mSpeed );
    vec3 rand_vec = vec3( rv(), rv(), rv() );

    Utility::RandomFloat rt( time - 2.0f, time + 2.0f );
    mParticles.push_back( std::make_shared<cParticle>( rand_vec, position, rt() ) );
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

    sortByCamera<cParticleHolder>( mParticleHolders, vec3( 0 ) );
}

void cParticleManager::draw()
{
    gl::ScopedGlslProg glsl( gl::getStockShader( gl::ShaderDef().texture() ) );
    for ( auto& it : mParticleHolders )
        it->draw( mBuilbordRotate );
}

void cParticleManager::create( const ci::vec3& position,
                               const ParticleType& type,
                               const ParticleTextureType& texture_type,
                               const float& time,
                               const float& speed )
{
    create( position, type, texture_type, time, 0, speed );
}

void cParticleManager::create( const ci::vec3 & position,
                               const ParticleType & type,
                               const ParticleTextureType & texture_type,
                               const float& time,
                               const int & count,
                               const float& speed )
{
    mParticleHolders.push_back(
        std::make_shared<cParticleHolder>( position, type, texture_type, time, count, speed ) );
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
