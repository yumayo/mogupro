#include <Game/Field/cBlock.h>
#include <cinder/gl/gl.h>
#include <Particle/cParticleManager.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cBlock::cBlock() :
    mCollider( vec3( 0 ), vec3( BLOCK_SIZE ), vec3( BLOCK_SIZE / 2.0f ) )
    , mType( BlockType::AIR )
{

}

cBlock::cBlock( const ci::vec3& position, const float& scale, const uint & id ) :
    mCollider( position, vec3( scale ) )
    , mType( BlockType::NORMAL )
{
}

cBlock::~cBlock()
{

}

void cBlock::setup()
{
    if ( mType != BlockType::AIR )
        mCollider.addWorld();
}

void cBlock::clear()
{
}

void cBlock::toBreak()
{
    mType = BlockType::AIR;
    clear();
    Particle::cParticleManager::getInstance()->
        create( getPosition(),
                Particle::ParticleType::EXPROTION,
                Particle::ParticleTextureType::SAND,
                vec3( 0.02f ),
                1.0f,
                1,
                0.2f );
    mCollider.removeWorld();
}
void cBlock::toRevival()
{
    mType = BlockType::NORMAL;
    mCollider.addWorld();
}
ci::vec3 cBlock::getPosition()
{
    return mCollider.getPosition();
}
BlockType cBlock::getType()
{
    return mType;
}
BlockType cBlock::setType( const BlockType & type )
{
    return mType = type;
}
int cBlock::getIndex()
{
    return mId;
}
bool cBlock::isActive()
{
    return mType != BlockType::AIR;
}
}
}