#include <Particle/cParticleManager.h>
#include <Resource/TextureManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cLightManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Utility/cRandom.h>
#include <cinder/ip/Fill.h>
#include <cinder/ip/Blend.h>
#include <Particle/Easing/cEase.h>

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

const std::array<GLfloat, 12> rect_vertex
{
    -1.0f / 2,  -1.0f / 2, 0,
    1.0f / 2, -1.0f / 2, 0,
    1.0f / 2,  1.0f / 2, 0,
    -1.0f / 2,  1.0f / 2, 0,
};

const std::array<ci::vec2, 4> tex_coords
{
    ci::vec2( 0.0 ,0.0 ),
    ci::vec2( 0.0 ,1.0 ),
    ci::vec2( 1.0 ,1.0 ),
    ci::vec2( 1.0 ,0.0 ),
};

//-------------------------------------------------------------------------------------------------------------------
//
// ParticleParam
//
//-------------------------------------------------------------------------------------------------------------------

ParticleParam::ParticleParam() :
    mPosition( NULL ),
    mCurrentPosition( vec3( 0 ) ),
    mScale( 1.0f ),
    mMoveType( ParticleType::EXPROTION ),
    mTextureType( ParticleTextureType::NONE ),
    mColor( ci::ColorA::white() ),
    mCount( 1 ),
    mVanishTime( 2.0f ),
    mVanishTimeRange( 1.0f ),
    mVainshBeginTime( 1.0f ),
    mEffectTime( 5.0f ),
    mSpeed( 0.2f ),
    mAddVec( ci::vec3( 0 ) ),
    mGravity( 0 ),
    mEffectRange( 3.0f ),
    mEaseTime( 30.0f ),
    mEaseType( EaseType::Linear ),
    mIsLighting( false ),
    mIsTrajectory( false ),
    mIsCube( false ),
    mConvergePoint( vec3( 0 ) ),
    mSwellEndTime( 0.0f ),
    mSwellWaitTime( 0.0f )
{
}

ParticleParam & ParticleParam::position( ci::vec3 & position )
{
    mPosition = &position;
    mCurrentPosition = position;
    return *this;
}
ParticleParam & ParticleParam::position( const ci::vec3 & position )
{
    mCurrentPosition = position;
    mPosition = NULL;
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
ParticleParam & ParticleParam::vanishBeginTime( const float & vanish_begin_time )
{
    mVainshBeginTime = vanish_begin_time;
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
ParticleParam & ParticleParam::gravity( const float & gravity )
{
    mGravity = gravity;
    return *this;
}
ParticleParam & ParticleParam::effectRange( const float & effect_range )
{
    mEffectRange = effect_range;
    return *this;
}
ParticleParam & ParticleParam::easeTime( const float & ease_time )
{
    mEaseTime = ease_time;
    return *this;
}
ParticleParam & ParticleParam::easeType( const EaseType & ease_type )
{
    mEaseType = ease_type;
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
ParticleParam & ParticleParam::isCube( const bool & is_cube )
{
    mIsCube = is_cube;
    return *this;
}
ParticleParam & ParticleParam::randomEaseTypes( const std::vector<EaseType>& ease_types )
{
    mEaseTypes = ease_types;
    return *this;
}
ParticleParam & ParticleParam::convergePoint( const ci::vec3& converge_point )
{
    mConvergePoint = converge_point;
    return *this;
}
ParticleParam & ParticleParam::swellEndTime( const float& swell_end_time )
{
    mSwellEndTime = swell_end_time;
    return *this;
}
ParticleParam & ParticleParam::swellWaitTime( const float & swell_wait_time )
{
    mSwellWaitTime = swell_wait_time;
    return *this;
}


//-------------------------------------------------------------------------------------------------------------------
//
// cParticle
//
//-------------------------------------------------------------------------------------------------------------------

cParticle::cParticle( const ci::vec3& position,
                      const ci::vec3& vec,
                      const float& time,
                      const float& vanish_begin_time ) :
    mPosition( position )
    , mPrevPosition( position )
    , mVec( vec )
    , mTime( time )
    , mVanishBeginTime( vanish_begin_time )
    , mTrajectoryCount( 10 )
    , mAlpha( 1.0f )
{

}

cParticle::~cParticle()
{
    Easing->kill( mPosition );
}

void cParticle::update( const float& delta_time, const float& gravity )
{
    mPrevPosition = mPosition;
    mPosition += mVec;

    mVec.y -= gravity;

    alphaUpdate( delta_time );

    mTime -= delta_time;
}

void cParticle::draw( const glm::quat& rotation, const ci::ColorA& color )
{
    Rectf rect( vec2( -0.5f, -0.5f ), vec2( 0.5f, 0.5f ) );

    gl::ScopedColor scoped_color( color.r, color.g, color.b, getAlpha() );

    gl::pushModelView();
    gl::translate( mPosition );

    glm::fmat4 mat = glm::toMat4( rotation );
    gl::multModelMatrix( mat );

    gl::drawSolidRect( rect );

    gl::popModelView();
}

void cParticle::cubeDraw( const ci::ColorA& color )
{
    gl::ScopedColor scoped_color( color.r, color.g, color.b, 1.0f );

    gl::pushModelView();
    gl::translate( mPosition );

    gl::drawCube( vec3( 0.0f ), vec3( 1.0f ) );

    gl::popModelView();
}

bool cParticle::isActive()
{
    return mTime > 0 || !Easing->isEaseEnd( mPosition );
}

ci::vec3 cParticle::getVec()
{
    return mPosition - mPrevPosition;
}

void cParticle::alphaUpdate( const float& delta_time )
{
    if ( mVanishBeginTime < mTime )
        return;

    const float& alpha_delta_time = delta_time / mVanishBeginTime;
    mAlpha -= alpha_delta_time;
}

float cParticle::getAlpha()
{
    if ( mVanishBeginTime < mTime )
        return 1.0f;
    return glm::clamp( mAlpha, 0.0f, 1.0f );
}


//-------------------------------------------------------------------------------------------------------------------
//
// cParticleHolder
//
//-------------------------------------------------------------------------------------------------------------------

cParticleHolder::cParticleHolder( const ParticleParam& param ) :
    mParam( param )
    , mIndicesIndex( 0 )
    , mVbo( nullptr )
    , mIsSwellEnd( false )
{
    setTexture( param.mTextureType );
    setLight( param.mIsLighting );

    mMesh = TriMesh::create( TriMesh::Format().positions().colors( 4 ).texCoords() );

    for ( int i = 0; i < param.mCount; i++ )
        createParticle();

    mVbo = gl::VboMesh::create( *mMesh );
}

cParticleHolder::cParticleHolder( vec3& position,
                                  const ParticleType& type,
                                  const ParticleTextureType& texture_type,
                                  const float& scale,
                                  const float& time,
                                  const int& count,
                                  const float& speed,
                                  const bool& lighting,
                                  const ci::ColorA& color )
{
    mParam.mCurrentPosition = position;
    mParam.mPosition = &position;
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
            createParticle();

    if ( mParam.mIsTrajectory )
        createTrajectory( vec3( 0 ), mParam.mVanishTime, 0.016f );
}

cParticleHolder::~cParticleHolder()
{
    for ( auto& it : mParticles )
        Easing->kill( it->mPosition );
    mParticles.clear();
    mTrajectoryParticles.clear();
}

void cParticleHolder::update( const float& delta_time )
{
    float errRange = 10.0f;
    if ( mParam.mPosition != NULL && mParam.mPosition != nullptr )
        if ( *mParam.mPosition == vec3( 0 ) ||
            ( mParam.mPosition->x < mParam.mCurrentPosition.x + errRange &&
              mParam.mPosition->x > mParam.mCurrentPosition.x - errRange &&
              mParam.mPosition->y < mParam.mCurrentPosition.y + errRange &&
              mParam.mPosition->y > mParam.mCurrentPosition.y - errRange &&
              mParam.mPosition->z < mParam.mCurrentPosition.z + errRange &&
              mParam.mPosition->z > mParam.mCurrentPosition.z - errRange ) )
            mParam.mCurrentPosition = *mParam.mPosition;

    if ( mParam.mMoveType == ParticleType::ABSORB )
    {
        if ( mIsSwellEnd == false && mParam.mSwellEndTime <= 0 )
        {
            mIsSwellEnd = true;
            // パーティクルの更新
            for ( auto& it : mParticles )
            {
                Utility::RandomInt ri( 0, mParam.mEaseTypes.size() );
                Easing->wait( it->mPosition,
                              mParam.mSwellWaitTime );
                Easing->add( it->mPosition,
                             *mParam.mPosition - mParam.mConvergePoint,
                             mParam.mEaseTime,
                             mParam.mEaseTypes[ri()] );
            }
        }
        mParam.mSwellEndTime -= delta_time;
    }

    // パーティクルの更新
    for ( auto& it = mParticles.begin(); it != mParticles.end(); )
    {
        ( *it )->update( delta_time, mParam.mGravity );
        if ( ( *it )->isActive() == false )
            it = mParticles.erase( it );
        else
            it++;
    }
    if ( mParam.mEffectTime >= 0 )
        if ( mParam.mMoveType != ParticleType::EXPROTION )
            for ( int i = 0; i < mParam.mCount; i++ )
                createParticle();


    // 軌跡の更新
    for ( auto& it = mTrajectoryParticles.begin(); it != mTrajectoryParticles.end(); )
    {
        ( *it )->update( delta_time, 0.0f );
        if ( ( *it )->isActive() == false )
            it = mTrajectoryParticles.erase( it );
        else
            it++;
    }
    if ( mParam.mEffectTime >= 0 )
        if ( mParam.mIsTrajectory )
            createTrajectory( vec3( 0 ), mParam.mVanishTime, delta_time );

    // ホルダーの寿命
    mParam.mEffectTime -= delta_time;

    // パーティクルの生成
    if ( mParam.mIsCube == false )
    {
        clearMesh();
        createMesh( mRotation, mParticles );
        createMesh( mRotation, mTrajectoryParticles );
    }
}

void cParticleHolder::draw( const glm::quat& rotation )
{
    gl::pushModelView();
    gl::ScopedBlendAlpha blend;

    // テクスチャがある描画
    if ( mTextureName != "" )
    {
        gl::ScopedGlslProg glsl( gl::getStockShader( gl::ShaderDef().color().texture() ) );

        auto texture = TEX->get( mTextureName );
        if ( !texture ) return;
        gl::ScopedTextureBind tex( texture );
        particleDraw( rotation );
    }
    // テクスチャがない描画
    else
    {
        gl::ScopedGlslProg glsl( gl::getStockShader( gl::ShaderDef().color() ) );
        particleDraw( rotation );
    }
    gl::popModelView();
}

bool cParticleHolder::isActive()
{
    return mParticles.size() > 0 || mTrajectoryParticles.size() > 0;
}

void cParticleHolder::createParticle()
{
    const vec3& position = createPosition();
    const auto& rand_vec = createVec( mParam.mCurrentPosition + position );
    const float& vanish_time = createVanishTime();

    mParticles.emplace_back( std::make_shared<cParticle>( position,
                                                          rand_vec,
                                                          vanish_time,
                                                          mParam.mVainshBeginTime ) );

    if ( mParam.mMoveType == ParticleType::CONVERGE )
        Easing->add( mParticles.back()->mPosition,
                     vec3( 0 ),
                     mParam.mEaseTime,
                     mParam.mEaseType );
}

ci::vec3 cParticleHolder::createPosition()
{
    if ( mParam.mMoveType == ParticleType::GLITTER ||
         mParam.mMoveType == ParticleType::CONVERGE )
    {
        const float& range = mParam.mEffectRange;
        Utility::RandomFloat rr( -range, range );
        vec3 rand_pos( rr(), rr(), rr() );
        rand_pos = glm::normalize( rand_pos );

        rand_pos *= rr();
        return rand_pos;
    }
    return vec3( 0 );
}

ci::vec3 cParticleHolder::createVec( const ci::vec3& particle_position )
{
    vec3 vec( 0 );
    if ( mParam.mMoveType == ParticleType::EXPROTION ||
         mParam.mMoveType == ParticleType::SCATTER )
    {
        Utility::RandomFloat rv( -1, 1 );
        vec = vec3( rv(), rv(), rv() );
        vec = glm::normalize( vec );
        vec *= mParam.mSpeed;
    }
    else if ( mParam.mMoveType == ParticleType::CONVERGE )
    {
        //vec3 center_vec = mParam.mCurrentPosition - particle_position;
        //vec = glm::normalize( center_vec );
        //vec *= mParam.mSpeed;
    }
    else if ( mParam.mMoveType == ParticleType::GLITTER )
    {
    }
    vec += mParam.mAddVec;
    return vec;
}

float cParticleHolder::createVanishTime()
{
    Utility::RandomFloat rt( mParam.mVanishTime, mParam.mVanishTime + mParam.mVanishTimeRange );
    return rt();
}

void cParticleHolder::createTrajectory( const ci::vec3 & position,
                                        const float & vanish_time,
                                        const float& delta_time )
{
    for ( size_t i = 0; i < mParticles.size(); i++ )
    {
        const vec3& c_vec = mParticles[i]->mVec;
        const float& length = glm::length( c_vec * 2.0f );
        const float& count = ceil( length / mParam.mScale );
        const vec3& one_vec = c_vec / (float) count;

        const vec3& prev_pos = mParticles[i]->mPrevPosition;

        const float& prev_time = delta_time * (float) ( ( mParticles[i]->mTrajectoryCount - 1 ) / 2.0f );
        const float& current_time = delta_time * (float) ( mParticles[i]->mTrajectoryCount / 2.0f );
        const float& time_vec = current_time - prev_time;
        const float& one_time_vec = time_vec / count;

        for ( int i = 0; i < count; i++ )
        {
            const float& current_time = prev_time + ( one_time_vec * (float) i );
            const vec3& t_pos = prev_pos + ( one_vec * (float) i );
            mTrajectoryParticles.push_back( std::make_shared<cParticle>( t_pos,
                                                                         vec3( 0 ),
                                                                         current_time,
                                                                         mParam.mVainshBeginTime ) );
        }

        mParticles[i]->mTrajectoryCount++;
    }
}

void cParticleHolder::createMesh( const glm::quat& rotation,
                                  std::vector<std::shared_ptr<cParticle>>& particles )
{
    for ( auto& it : particles )
    {
        mMesh->appendTexCoords0( &tex_coords[0], 4 );
        for ( int i = 0, index = 0; i < 4; i++ )
        {
            vec3 vertex( 0 );
            vertex.x += rect_vertex[index++];
            vertex.y += rect_vertex[index++];
            vertex.z += rect_vertex[index++];

            vertex = glm::rotate( rotation, vertex );
            vertex += it->mPosition;

            mMesh->appendPosition( vertex );

            // カラーを生成時に指定できるようにする
            if ( mParam.mColors.size() > i )
                mMesh->appendColorRgba( ColorA( mParam.mColors[i].r,
                                                mParam.mColors[i].g,
                                                mParam.mColors[i].b, it->getAlpha() ) );
            else
                mMesh->appendColorRgba( ColorA( mParam.mColor.r,
                                                mParam.mColor.g,
                                                mParam.mColor.b, it->getAlpha() ) );

        }

        auto & indices = mMesh->getIndices();
        indices.insert( indices.end(),
        {
            mIndicesIndex,
            mIndicesIndex + 1,
            mIndicesIndex + 2,

            mIndicesIndex + 2,
            mIndicesIndex + 3,
            mIndicesIndex
        } );
        mIndicesIndex += 4;
    }

    if ( mVbo != nullptr )
        mVbo = gl::VboMesh::create( *mMesh );
}

void cParticleHolder::particleDraw( const glm::quat& rotation )
{
    gl::translate( mParam.mCurrentPosition );
    gl::scale( vec3( mParam.mScale ) );
    if ( mParam.mIsCube == false )
    {
        auto ctx = gl::context();
        const gl::GlslProg* curGlslProg = ctx->getGlslProg();

        //ctx->pushVao();
        ctx->getDefaultVao()->replacementBindBegin();
        mVbo->buildVao( curGlslProg );
        ctx->getDefaultVao()->replacementBindEnd();
        ctx->setDefaultShaderVars();
        mVbo->drawImpl();
        //ctx->popVao();
        //gl::draw( mVbo );
    }
    else
    {
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
}

void cParticleHolder::setTexture( const ParticleTextureType & texture_type )
{
    if ( texture_type != ParticleTextureType::NONE )
    {
        mTextureName = getTextureNameFromTextureType( texture_type );
        TEX->set( mTextureName, "Particle/" + mTextureName + ".png" );
    }
}

void cParticleHolder::setLight( bool is_lighting )
{
    if ( is_lighting )
        mHandle = Game::cLightManager::getInstance()->addPointLight( mParam.mCurrentPosition,
                                                                     vec3( 1, 1, 1 ),
                                                                     2.0f );
}

void cParticleHolder::clearMesh()
{
    mMesh->clear();
    mIndicesIndex = 0;
}


//-------------------------------------------------------------------------------------------------------------------
//
// cParticleManager
//
//-------------------------------------------------------------------------------------------------------------------

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
    Easing->update( delta_time );
    builbordUpdate();
    for ( auto& it = mParticleHolders.begin(); it != mParticleHolders.end(); )
    {
        ( *it )->mRotation = mBuilbordRotate;
        ( *it )->update( delta_time );
        if ( ( *it )->isActive() == false )
            it = mParticleHolders.erase( it );
        else
            it++;
    }
}

void cParticleManager::draw()
{
    gl::disableDepthWrite();

    auto ctx = gl::context();
    bool is_culling_change = ctx->getBoolState( GL_CULL_FACE );

    if ( is_culling_change == false )
        gl::enableFaceCulling( true );

    for ( auto& it : mParticleHolders )
        it->draw( mBuilbordRotate );

    if ( is_culling_change == false )
        gl::enableFaceCulling( false );

    gl::enableDepthWrite();
}

void cParticleManager::create( const ParticleParam& param )
{
    mParticleHolders.emplace_back( std::make_shared<cParticleHolder>( param ) );
}

void cParticleManager::create( ci::vec3 & position,
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

void cParticleManager::create( ci::vec3 & position,
                               const ParticleType & type,
                               const ParticleTextureType & texture_type,
                               const ci::vec3 & scale,
                               const float & time,
                               const int & count,
                               const float & speed,
                               const bool & lighting,
                               const ci::ColorA & color )
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
