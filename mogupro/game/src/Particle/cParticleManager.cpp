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

//template<typename T>
//void sortByCamera( std::vector<std::shared_ptr<T>>& list, const vec3& offset_pos )
//{
//    vec3 camera_pos = CameraManager::cCameraManager::getInstance()->getCamera().getEyePoint();
//
//    for ( size_t i = 0; i < list.size(); i++ )
//    {
//        for ( size_t k = 0; k < list.size() - i - 1; k++ )
//        {
//            const vec3& p1_pos = offset_pos + list[k]->mPosition;
//            const vec3& p2_pos = offset_pos + list[k + 1]->mPosition;
//
//            float length1 = getLength( camera_pos, p1_pos );
//            float length2 = getLength( camera_pos, p2_pos );
//
//            if ( length1 < length2 )
//                std::swap( list[k], list[k + 1] );
//        }
//    }
//}

ParticleParam::ParticleParam() :
    mPosition( ci::vec3( 0 ) ),
    mScale( ci::vec3( 1 ) ),
    mMoveType( ParticleType::EXPROTION ),
    mTextureType( ParticleTextureType::NONE ),
    mColor( ci::ColorA::white() ),
    mCount( 10 ),
    mVanishTime( 2.0f ),
    mEffectTime( 5.0f ),
    mSpeed( 0.2f ),
    mIsLighting( false ),
    mIsTrajectory( false )
{
}
ParticleParam & ParticleParam::position( const ci::vec3 & position )
{
    mPosition = position;
    return *this;
}
ParticleParam & ParticleParam::scale( const ci::vec3 & scale )
{
    mScale = scale;
    return *this;
}
ParticleParam & ParticleParam::moveType( const ParticleType & move_type )
{
    mMoveType = move_type;
    return *this;
}
ParticleParam & ParticleParam::textureType( const ParticleTextureType & texture_type )
{
    mTextureType = texture_type;
    return *this;
}
ParticleParam & ParticleParam::color( const ci::ColorA & color )
{
    mColor = color;
    return *this;
}
ParticleParam & ParticleParam::count( const int & count )
{
    mCount = count;
    return *this;
}
ParticleParam & ParticleParam::vanishTime( const float & vanish_time )
{
    mVanishTime = vanish_time;
    return *this;
}
ParticleParam & ParticleParam::effectTime( const float & effect_time )
{
    mEffectTime = effect_time;
    return *this;
}
ParticleParam & ParticleParam::speed( const float & speed )
{
    mSpeed = speed;
    return *this;
}
ParticleParam & ParticleParam::isLighting( const bool & is_lighting )
{
    mIsLighting = is_lighting;
    return *this;
}
ParticleParam & ParticleParam::isTrajectory( const bool & is_trajectory )
{
    mIsTrajectory = is_trajectory;
    return *this;
}

cParticle::cParticle( const ci::vec3& vec,
                      const ci::vec3& position,
                      const ci::vec3& scale,
                      const float& time ) :
    mPosition( position )
    , mVec( vec )
    , mTime( time )
{
    mLineLengthCount = 3;
    vec3 max_vec = mVec * (float) mLineLengthCount;
    float length = glm::length( max_vec );

    float line_num = length / ( scale.x * 2 );
    mOneLineVec = max_vec / line_num;
    mLineCount = line_num;
}

cParticle::~cParticle()
{
    mLinePositions.clear();
}

void cParticle::update( const float& delta_time )
{
    mPosition += mVec;
    mTime -= delta_time;
}

void cParticle::draw( const glm::quat& rotation, const ci::ColorA& color )
{
    Rectf rect( vec2( -0.5, -0.5 ), vec2( 0.5, 0.5 ) );

    for ( size_t i = 0; i < mLinePositions.size(); i++ )
    {
        float alpha = clamp( mTime, 0.0f, 1.0f );
        alpha *= i / (float) mLinePositions.size();

        gl::ScopedColor color( color.r, color.g, color.b, alpha );

        gl::pushModelView();
        gl::translate( mLinePositions[i] );

        glm::fmat4 mat = glm::toMat4( rotation );
        gl::multModelMatrix( mat );

        gl::drawSolidRect( rect );

        gl::popModelView();
    }
}

bool cParticle::isActive()
{
    return mTime > 0;
}

void cParticle::trajectoryUpdate( const bool& is_trajectory )
{
    if ( is_trajectory )
    {
        if ( mLinePositions.size() > 1 )
            for ( int i = 0; i < mLineCount - 1; i++ )
                mLinePositions.pop_back();

        for ( int i = 1; i < mLineCount; i++ )
        {
            vec3 offset = mOneLineVec * (float) i;
            mLinePositions.emplace_back( mPosition + offset );
        }
    }
    else
    {
        mLinePositions.pop_back();
    }

}

cParticleHolder::cParticleHolder( const ParticleParam& param ) :
    mParam( param )
{
    setTexture( param.mTextureType );
    setLight( param.mIsLighting );

    if ( param.mMoveType == ParticleType::EXPROTION )
        for ( int i = 0; i < param.mCount; i++ )
            create( vec3( 0 ), param.mVanishTime );
}
cParticleHolder::cParticleHolder( const vec3& position,
                                  const ParticleType& type,
                                  const ParticleTextureType& texture_type,
                                  const ci::vec3& scale,
                                  const float& time,
                                  const int& count,
                                  const float& speed,
                                  const bool& lighting,
                                  const ci::ColorA& color )
{
    mParam.mPosition = position;
    mParam.mMoveType = type;
    mParam.mTextureType = texture_type;
    mParam.mScale = scale;
    mParam.mVanishTime = time;
    mParam.mEffectTime = time;
    mParam.mCount = count;
    mParam.mIsLighting = lighting;
    mParam.mIsTrajectory = true;
    mParam.mColor = color;

    setTexture( mParam.mTextureType );
    setLight( mParam.mIsLighting );

    if ( mParam.mMoveType == ParticleType::EXPROTION )
        for ( int i = 0; i < mParam.mCount; i++ )
            create( vec3( 0 ), mParam.mVanishTime );
}

cParticleHolder::~cParticleHolder()
{
    if ( mParam.mIsLighting )
        Game::cLightManager::getInstance()->removePointLight( mHandle );
    mParticles.clear();
}

void cParticleHolder::update( const float& delta_time )
{
    mParam.mEffectTime -= delta_time;
    if ( mParam.mEffectTime > 0 )
        if ( mParam.mMoveType != ParticleType::EXPROTION )
            create( vec3( 0 ), mParam.mVanishTime );

    for ( auto& it = mParticles.begin(); it != mParticles.end(); )
    {
        ( *it )->trajectoryUpdate( mParam.mIsTrajectory );
        ( *it )->update( delta_time );
        if ( ( *it )->isActive() == false )
            it = mParticles.erase( it );
        else
            it++;
    }

    //sort();
}

void cParticleHolder::draw( const glm::quat& rotation )
{
    gl::pushModelView();
    gl::ScopedBlendAlpha blend;

    if ( mTextureName != "" )
    {
        gl::ScopedGlslProg glsl( gl::getStockShader( gl::ShaderDef().color().texture() ) );

        auto texture = TEX->get( mTextureName );
        if ( !texture ) return;
        gl::ScopedTextureBind tex( texture );
        particleDraw( rotation );
    }
    else
    {
        gl::ScopedGlslProg glsl( gl::getStockShader( gl::ShaderDef().color() ) );
        particleDraw( rotation );
    }
    gl::popModelView();
}

bool cParticleHolder::isActive()
{
    return mParticles.size() > 0;
}

void cParticleHolder::sort()
{
    //sortByCamera<cParticle>( mParticles, mParam.mPosition );
}

void cParticleHolder::create( const ci::vec3& position,
                              const float& time )
{
    Utility::RandomFloat rv( -mParam.mSpeed, mParam.mSpeed );
    vec3 rand_vec = vec3( rv(), rv(), rv() );
    rand_vec = glm::normalize( rand_vec );
    rand_vec += rv();
    Utility::RandomFloat rt( time, time + 1.0f );
    mParticles.emplace_back( std::make_shared<cParticle>( rand_vec, position, mParam.mScale, rt() ) );
}

void cParticleHolder::particleDraw( const glm::quat& rotation )
{
    gl::translate( mParam.mPosition );
    gl::scale( mParam.mScale );
    for ( const auto& it : mParticles )
    {
        it->draw( rotation, mParam.mColor );
    }
}

void cParticleHolder::setTexture( const ParticleTextureType & texture_type )
{
    if ( texture_type != ParticleTextureType::NONE )
    {
        mTextureName = getTextureNameFromTextureType( texture_type );
        TEX->set( mTextureName, mTextureName + ".png" );
    }
}

void cParticleHolder::setLight( bool is_lighting )
{
    if ( is_lighting )
        mHandle = Game::cLightManager::getInstance()->addPointLight( mParam.mPosition,
                                                                     vec3( 1, 1, 1 ),
                                                                     2.0f );
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

    //sortByCamera<cParticleHolder>( mParticleHolders, vec3( 0 ) );
}

void cParticleManager::draw()
{
    gl::disableDepthWrite();

    for ( auto& it : mParticleHolders )
        it->draw( mBuilbordRotate );
    gl::enableDepthWrite();
}

void cParticleManager::create( const ParticleParam& param )
{
    mParticleHolders.emplace_back( std::make_shared<cParticleHolder>( param ) );
}

void cParticleManager::create( const ci::vec3 & position,
                               const ParticleType & type,
                               const ParticleTextureType & texture_type,
                               const ci::vec3 & scale,
                               const float & time,
                               const int & count,
                               const float & speed,
                               const bool & lighting,
                               const ci::ColorA & color )
{
    mParticleHolders.emplace_back( std::make_shared<cParticleHolder>( position, type, texture_type, scale, time, count, speed, lighting, color ) );
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
