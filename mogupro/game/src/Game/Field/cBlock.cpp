#include <Game/Field/cBlock.h>
#include <cinder/gl/gl.h>

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
    mCollider.removeWorld();
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
bool cBlock::isActive()
{
    return mType != BlockType::AIR;
}
}
}