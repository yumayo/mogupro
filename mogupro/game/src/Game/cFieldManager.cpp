#include <Game/cFieldManager.h>
#include <cinder/gl/gl.h>
#include "Sound/Stereophonic.h"
#include <Game/Field/FieldData.h>
using namespace ci;
using namespace ci::app;

namespace Game
{
cFieldManager::cFieldManager()
{
}
cFieldManager::~cFieldManager()
{
}
void cFieldManager::setup()
{
    mUnderGround = std::make_shared<Field::cUnderGround>();
    mUnderGround->setup();
}
void cFieldManager::update( const float& delta_time )
{
    mUnderGround->update( delta_time );
}
void cFieldManager::draw()
{
    auto ctx = gl::context();
    bool is_culling_change = ctx->getBoolState( GL_CULL_FACE );

    if ( is_culling_change == false )
        gl::enableFaceCulling( true );

    mUnderGround->draw();

    if ( is_culling_change == false )
        gl::enableFaceCulling( false );
}
bool cFieldManager::blockBreak( const ci::vec3& position, const float& radius, const Field::cBreakBlockType& type )
{
    return mUnderGround->blockBreak( position, radius, type );
}
bool cFieldManager::blockBreakNetwork( const ci::vec3 & position, const float & radius, const Field::cBreakBlockType& type )
{
    return mUnderGround->blockBreakNetwork( position, radius, type );
}
bool cFieldManager::isBreakBlock( const ci::vec3 & position, const float & radius )
{
    return mUnderGround->isBreakBlock( position, radius );
}
ci::vec3 cFieldManager::getBlockHighestPosition( const ci::vec3 & target_position )
{
    return mUnderGround->getBlockHighestPosition( target_position );
}
ci::vec3 cFieldManager::getBlockTopPosition( const ci::vec3 & target_position )
{
    return mUnderGround->getBlockTopPosition( target_position );
}
std::vector<int> cFieldManager::getChunkId( const ci::vec3 & position, const float & radius )
{
    return mUnderGround->getChunkId( position, radius );
}
bool cFieldManager::isUnderCannon( const int & chunk_x, const int & chunk_y, const int & chunk_z )
{
    using namespace Field;
    ivec3 cell = ivec3( chunk_x, chunk_y, chunk_z );
    if ( cell.x < 3 || cell.x > 5 )
        return false;
    if ( cell.z > 2 && cell.z + 1 < CHUNK_RANGE_Z - 2 )
        return false;
    return true;
}
void cFieldManager::blockAllReset()
{
    mUnderGround->blockAllReset();
}
void cFieldManager::shutdown()
{
    mUnderGround->shutdown();
}
void cFieldManager::addToBreakType( const Field::BlockType & type )
{
    if ( mToBreakBlocksType.find( type ) != mToBreakBlocksType.end() )
        return;
    mToBreakBlocksType.insert( type );
}
void cFieldManager::clearToBreakType()
{
    mToBreakBlocksType.clear();
}
void cFieldManager::playBreakBlockSounds( const ci::vec3& position, const std::string& key )
{
    vec3 pos = position;
    if ( mToBreakBlocksType.find( Field::BlockType::AIR ) != mToBreakBlocksType.end() )
    {
        //Sound::StereophonicManager::getInstance()->add( key, ci::app::getAssetPath( "SE/Player/drill.wav" ).string(), pos );
    }
    if ( mToBreakBlocksType.find( Field::BlockType::UNBREAKING ) != mToBreakBlocksType.end() )
    {
        Sound::StereophonicManager::getInstance()->add( key, ci::app::getAssetPath( "SE/Player/kinnzoku.wav" ).string(), pos );
    }
    else if ( mToBreakBlocksType.find( Field::BlockType::NORMAL ) != mToBreakBlocksType.end() )
    {
        Sound::StereophonicManager::getInstance()->add( key, ci::app::getAssetPath( "SE/Player/drill.wav" ).string(), pos );
    }
    clearToBreakType();
}
}