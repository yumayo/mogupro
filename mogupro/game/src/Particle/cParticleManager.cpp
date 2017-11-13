#include <Particle/cParticleManager.h>
#include <Resource/TextureManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cLightManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Utility/cRandom.h>

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

cParticle::cParticle( const ci::vec3& vec, const ci::vec3& position ) :
    mPosition( position )
    , mVec( vec )
{
}

void cParticle::update()
{
    mPosition += mVec;
}

cParticleHolder::cParticleHolder( const vec3& position,
                                  const ParticleType& type,
                                  const ParticleTextureType& texture_type,
                                  const float& time ) :
    mTime( time )
{
    mTextureName = getTextureNameFromTextureType( texture_type );
    TEX->set( mTextureName, mTextureName + ".png" );

    mHandle = Game::cLightManager::getInstance()->addPointLight( mPosition,
                                                                 vec3( 1, 1, 1 ),
                                                                 2.0f );
    create();
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
    create();

    for ( const auto& it : mParticles )
        it->update();
}

void cParticleHolder::draw( const glm::quat& rotation )
{
    gl::pushModelView();

    auto texture = TEX->get( mTextureName );
    if ( !texture )
        return;

    using namespace ci::gl;
    auto ctx = context();
    Rectf texRect = texture->getAreaTexCoords( Area( vec2( 0 ), texture->getSize() ) );
    ScopedVao vaoScp( ctx->getDrawTextureVao() );
    ScopedBuffer vboScp( ctx->getDrawTextureVbo() );
    ScopedTextureBind texBindScope( texture );

    for ( const auto& it : mParticles )
    {
        gl::pushModelView();
        gl::translate( it->mPosition );

        glm::fmat4 mat = glm::toMat4( rotation );
        gl::multModelMatrix( mat );

        Rectf rect( vec2( -0.5, 1.0 ), vec2( 0.5, 0.0 ) );
        gl::drawSolidRect( rect );

        gl::popModelView();
    }
    gl::popModelView();
}

bool cParticleHolder::IsActive()
{
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

void cParticleHolder::create()
{
    float range = 1.0f;
    Utility::RandomFloat r( -range, range );
    vec3 rand_vec = vec3( r(), r(), r() );
    mParticles.push_back( std::make_shared<cParticle>( rand_vec ) );
}

cParticleManager::cParticleManager()
{
}
cParticleManager::~cParticleManager()
{
}

void cParticleManager::update( const float& delta_time )
{
    builbordUpdate();
    for ( auto& it = mParticleHolders.begin(); it != mParticleHolders.end(); )
    {
        ( *it )->update( delta_time );
        if ( ( *it )->IsActive() == false )
            it = mParticleHolders.erase( it );
        else
            it++;
    }
}

void cParticleManager::draw()
{
    for ( auto& it : mParticleHolders )
        it->draw( mBuilbordRotate );
}

void cParticleManager::create( const ci::vec3& position,
                               const ParticleType& type,
                               const ParticleTextureType& texture_type,
                               const float& time )
{
    cParticleHolder holder = cParticleHolder( position, type, texture_type, time );
    mParticleHolders.push_back( std::make_shared<cParticleHolder>( holder ) );
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
