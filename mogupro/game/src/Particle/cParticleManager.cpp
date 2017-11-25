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
    return glm::length( vec );
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
    mScale( 1.0f ),
    mMoveType( ParticleType::EXPROTION ),
    mTextureType( ParticleTextureType::NONE ),
    mColor( ci::ColorA::white() ),
    mCount( 1 ),
    mVanishTime( 2.0f ),
    mVanishTimeRange( 1.0f ),
    mEffectTime( 5.0f ),
    mSpeed( 0.2f ),
    mAddVec( ci::vec3( 0 ) ),
    mIsLighting( false ),
    mIsTrajectory( false ),
    mGravity( 0 ),
    mIsCube( false )
{
}
ParticleParam & ParticleParam::position( const ci::vec3 & position )
{
    mPosition = position;
    return *this;
}
ParticleParam & ParticleParam::scale( const float & scale )
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
ParticleParam & ParticleParam::vanishTimeRange( const float & vanish_time_range )
{
    mVanishTimeRange = vanish_time_range;
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
ParticleParam & ParticleParam::addVec( const ci::vec3 & add_vec )
{
    mAddVec = add_vec;
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
ParticleParam & ParticleParam::gravity( const float & gravity )
{
    mGravity = gravity;
    return *this;
}
ParticleParam & ParticleParam::isCube( const bool & is_cube )
{
    mIsCube = is_cube;
    return *this;
}

cParticle::cParticle( const ci::vec3& position,
                      const ci::vec3& vec,
                      const float& time ) :
    mPosition( position )
    , mVec( vec )
    , mTime( time )
    , mTrajectoryCount( 1 )
{

}

cParticle::~cParticle()
{
}

void cParticle::update( const float& delta_time, const float& gravity )
{
    mPrevPosition = mPosition;
    mPosition += mVec;
    mVec.y -= gravity;

    mTime -= delta_time;
}

void cParticle::draw( const glm::quat& rotation, const ci::ColorA& color )
{
    Rectf rect( vec2( -0.5, -0.5 ), vec2( 0.5, 0.5 ) );

    gl::ScopedColor scoped_color( color.r, color.g, color.b, glm::clamp( mTime, 0.0f, 1.0f ) );

    gl::pushModelView();
    gl::translate( mPosition );

    glm::fmat4 mat = glm::toMat4( rotation );
    gl::multModelMatrix( mat );

    gl::drawSolidRect( rect );

    gl::popModelView();
}

void cParticle::cubeDraw( const ci::ColorA& color )
{
    gl::ScopedColor scoped_color( color.r, color.g, color.b, glm::clamp( mTime, 0.0f, 1.0f ) );

    gl::pushModelView();
    gl::translate( mPosition );

    gl::drawCube( vec3( 0.0f ), vec3( 1.0f ) );

    gl::popModelView();
}

bool cParticle::isActive()
{
    return mTime > 0;
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
                                  const float& scale,
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
    mParam.mVanishTimeRange = time;
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

    if ( mParam.mIsTrajectory )
        trajectoryCreate( vec3( 0 ), mParam.mVanishTime, 0.016f );
}

cParticleHolder::~cParticleHolder()
{
    if ( mParam.mIsLighting )
        Game::cLightManager::getInstance()->removePointLight( mHandle );
    mParticles.clear();
    mTrajectoryParticles.clear();
}

void cParticleHolder::update( const float& delta_time )
{
    if ( mParam.mEffectTime >= 0 )
        if ( mParam.mMoveType != ParticleType::EXPROTION )
        {
            for ( int i = 0; i < mParam.mCount; i++ )
                create( vec3( 0 ), mParam.mVanishTime );
        }


    for ( auto& it = mParticles.begin(); it != mParticles.end(); )
    {
        ( *it )->update( delta_time, mParam.mGravity );
        if ( ( *it )->isActive() == false )
            it = mParticles.erase( it );
        else
            it++;
    }

    if ( mParam.mIsTrajectory )
        trajectoryCreate( vec3( 0 ), mParam.mVanishTime, delta_time );

    for ( auto& it = mTrajectoryParticles.begin(); it != mTrajectoryParticles.end(); )
    {
        ( *it )->update( delta_time, 0.0f );
        if ( ( *it )->isActive() == false )
            it = mTrajectoryParticles.erase( it );
        else
            it++;
    }

    //sort();
    mParam.mEffectTime -= delta_time;
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
    Utility::RandomFloat rv( -1, 1 );
    vec3 rand_vec = vec3( rv(), rv(), rv() );
    rand_vec = glm::normalize( rand_vec );
    rand_vec *= mParam.mSpeed;
    rand_vec += mParam.mAddVec;
    Utility::RandomFloat rt( time, time + mParam.mVanishTimeRange );
    mParticles.emplace_back( std::make_shared<cParticle>( position, rand_vec, rt() ) );
}

void cParticleHolder::trajectoryCreate( const ci::vec3 & position,
                                        const float & vanish_time,
                                        const float& delta_time )
{
    for ( size_t i = 0; i < mParticles.size(); i++ )
    {
        const vec3& c_vec = mParticles[i]->mVec;
        float length = glm::length( c_vec * 2.0f );
        float count = ceil( length / mParam.mScale );
        const vec3& one_vec = c_vec / (float) count;

        const vec3& prev_pos = mParticles[i]->mPrevPosition;

        float prev_time = delta_time * (float) ( ( mParticles[i]->mTrajectoryCount - 1 ) / 2.0f );
        float current_time = delta_time * (float) ( mParticles[i]->mTrajectoryCount / 2.0f );
        float time_vec = current_time - prev_time;
        float one_time_vec = time_vec / count;

        for ( int i = 0; i < count; i++ )
        {
            float current_time = prev_time + ( one_time_vec * (float) i );
            vec3 t_pos = prev_pos + ( one_vec * (float) i );
            mTrajectoryParticles.push_back( std::make_shared<cParticle>( t_pos, vec3( 0 ), current_time ) );
        }

        mParticles[i]->mTrajectoryCount++;

    }
}

void cParticleHolder::particleDraw( const glm::quat& rotation )
{
    gl::translate( mParam.mPosition );
    gl::scale( vec3( mParam.mScale ) );

    for ( const auto& it : mParticles )
        if ( mParam.mIsCube == false )
            it->draw( rotation, mParam.mColor );
        else
            it->cubeDraw( mParam.mColor );

    for ( const auto& it : mTrajectoryParticles )
        if ( mParam.mIsCube == false )
            it->draw( rotation, mParam.mColor );
        else
            it->cubeDraw( mParam.mColor );
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
                               const float & scale,
                               const float & time,
                               const int & count,
                               const float & speed,
                               const bool & lighting,
                               const ci::ColorA & color )
{
    mParticleHolders.emplace_back( std::make_shared<cParticleHolder>( position, type, texture_type, scale, time, count, speed, lighting, color ) );
}

void cParticleManager::create( const ci::vec3 & position, const ParticleType & type, const ParticleTextureType & texture_type, const ci::vec3 & scale, const float & time, const int & count, const float & speed, const bool & lighting, const ci::ColorA & color )
{
    //mParticleHolders.emplace_back( std::make_shared<cParticleHolder>( position, type, texture_type, scale, time, count, speed, lighting, color ) );

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
