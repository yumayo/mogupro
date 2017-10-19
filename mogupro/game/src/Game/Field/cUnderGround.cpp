#include <Game/Field/cUnderGround.h>
#include <Resource/TextureManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Network/cUDPManager.h>
#include <Network/cRequestManager.h>
#include <Game/Field/cCalculateTriMesh.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cUnderGround::cUnderGround()
{
}
cUnderGround::~cUnderGround()
{
}
void cUnderGround::setup()
{
    TEX.set( "dirt", "dirt.jpg" );

    mBlockMaxCell = ivec3( 4, 4, 4 );
    mChunkNum = ivec3( 4, 0, 4 );
    mIntervalOffset = 0.0f;
    mScale = 1.0f;
    mOffset = vec3( mScale / 2 );
    createUnderGround();
}
void cUnderGround::update()
{
}
void cUnderGround::draw()
{
    //auto texture = TEX.get( "dirt" );
    //if (!texture)
    //    return;
    //using namespace ci::gl;

    //auto ctx = context();

    //Rectf texRect = texture->getAreaTexCoords( Area( vec2( 0 ), texture->getSize() ) );

    //ScopedVao vaoScp( ctx->getDrawTextureVao() );
    //ScopedBuffer vboScp( ctx->getDrawTextureVbo() );
    //ScopedTextureBind texBindScope( texture );

    //auto glsl = getStockShader( ShaderDef().uniformBasedPosAndTexCoord().color().texture( texture ) );
    //ScopedGlslProg glslScp( glsl );
    //glsl->uniform( "uTex0", 0 );
    //glsl->uniform( "uPositionOffset", vec2( 0 ) );
    //glsl->uniform( "uPositionScale", vec2( 1 ) );
    //glsl->uniform( "uTexCoordOffset", texRect.getUpperLeft() );
    //glsl->uniform( "uTexCoordScale", texRect.getSize() );

    gl::draw( mVboMesh );
}
bool cUnderGround::createUnderGround()
{
    uint count = 0;

    cTimeMeasurement::getInstance()->make();

    mBlocks = std::vector<std::vector<std::vector<std::shared_ptr<cBlock>>>>(
        mBlockMaxCell.z * mChunkNum.z, std::vector<std::vector<std::shared_ptr<cBlock>>>(
            mBlockMaxCell.y, std::vector<std::shared_ptr<cBlock>>(
                mBlockMaxCell.x * mChunkNum.x ) ) );

    for ( uint cz = 0; cz < mChunkNum.z; cz++ )
    {
        for ( uint cx = 0; cx < mChunkNum.x; cx++ )
        {
            uint sz = cz * mBlockMaxCell.z;
            uint sx = cx * mBlockMaxCell.x;
            for ( uint z = sz; z < mBlockMaxCell.z * mChunkNum.z; z++ )
            {
                if ( z / mBlockMaxCell.z > sz / mBlockMaxCell.z )
                    break;
                for ( uint y = 0; y < mBlockMaxCell.y; y++ )
                {
                    for ( uint x = sx; x < mBlockMaxCell.x * mChunkNum.x; x++ )
                    {
                        if ( x / mBlockMaxCell.x > sx / mBlockMaxCell.x )
                            break;

                        auto position = vec3( x * mScale + x * mIntervalOffset,
                                              y * mScale + y * mIntervalOffset,
                                              z * mScale + z * mIntervalOffset ) + mOffset;
                        std::shared_ptr<cBlock> block = std::make_shared<cBlock>( position, mScale, count );
                        block->calcMeshIndexData( count++ );

                        mBlocks[z][y][x] = block;
                    }
                }
            }
        }
    }

    createMesh();

    cTimeMeasurement::getInstance()->make();

    app::console() << std::endl << std::endl;
    app::console() << "Field create time : " << cTimeMeasurement::getInstance()->deltaTime() << " sec" << std::endl;
    app::console() << std::endl << std::endl;

    return true;
}
bool cUnderGround::createMesh()
{
    cCalculateTriMesh calc_trimesh;

    uint count = 0;
    for ( uint z = 0; z < mChunkNum.z; z++ )
    {
        for ( uint x = 0; x < mChunkNum.x; x++ )
        {
            cChunk chunk = calc_trimesh.calcChunk( mBlockMaxCell, ivec2( x, z ), count++, mScale );

            std::copy( chunk.mVertices.begin(), chunk.mVertices.end(), std::back_inserter( mVertices ) );
            std::copy( chunk.mIndices.begin(), chunk.mIndices.end(), std::back_inserter( mIndices ) );
            std::copy( chunk.mNormals.begin(), chunk.mNormals.end(), std::back_inserter( mNormals ) );
            std::copy( chunk.mUv.begin(), chunk.mUv.end(), std::back_inserter( mUv ) );
        }
    }

    mMesh = ci::TriMesh::create();
    if ( mVertices.size() > 0 )
        mMesh->appendPositions( &mVertices[0], mVertices.size() );
    if ( mIndices.size() > 0 )
        mMesh->appendIndices( &mIndices[0], mIndices.size() );
    if ( mUv.size() > 0 )
        mMesh->appendTexCoords0( &mUv[0], mUv.size() );
    if ( mNormals.size() > 0 )
        mMesh->appendNormals( &mNormals[0], mNormals.size() );
    mVboMesh = gl::VboMesh::create( *mMesh );

    return true;
}
void cUnderGround::blockMeshErase( std::shared_ptr<cBlock> b )
{
    auto indices = std::vector<uint>( b->mIndicesNum.size() * 4 );
    for ( uint i = 0; i < b->mIndicesNum.size(); i++ )
    {
        auto k = b->mIndicesNum[i];
        mIndices[k] = 0;
    }
    auto vbo = mVboMesh->getIndexVbo();
    // VboRef‚Ì’†‚ÅIndices‚ª4”{‚³‚ê‚é‚Ì‚ÅA•ÏX‰ÓŠ‚ð4”{‚·‚é
    vbo->bufferSubData( b->mIndicesNum[0] * 4, b->mIndicesNum.size() * 4, &indices[0] );
}
bool cUnderGround::blockDigged( const ci::ivec3& c )
{
    if ( isOutOfRange( c ) )
        return false;

    auto b = mBlocks[c.z][c.y][c.x];
    if ( b->mIsActive == false )
        return false;

    blockMeshErase( b );

    //using namespace Network;
    //using namespace Network::Packet;
    //Network::cUDPManager::getInstance()->
    //    send( cNetworkHandle( "126.77.126.180", 25565 ),
    //          new Request::cReqCheckBreakBlocks( c ) );
    return true;
}
void cUnderGround::blockAllClear()
{
    mVertices.clear();
    mIndices.clear();
    mUv.clear();
    mNormals.clear();
}
ci::ivec3 cUnderGround::getCellNumFromPosition( const ci::vec3 & position )
{
    return ivec3( ( position + mOffset ) / mScale );
}
bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    auto c = getCellNumFromPosition( position );

    if ( isOutOfRange( c ) )
        return false;

    auto r = ivec3( int( radius / mScale ) );
    auto s = c - r;
    auto e = c + r;

    for ( int z = s.z; z < e.z; z++ )
        for ( int y = s.y; y < e.y; y++ )
            for ( int x = s.x; x < e.x; x++ )
            {
                if ( isOutOfRange( ivec3( x, y, z ) ) )
                    continue;
                if ( blockDigged( ivec3( x, y, z ) ) )
                    mBlocks[z][y][x]->toBreak();
            }
    return true;
}
bool cUnderGround::blockBreak( const ci::ivec3 & cell_num, const float & radius )
{
    auto c = cell_num;
    if ( isOutOfRange( cell_num ) )
        return false;
    if ( blockDigged( ivec3( c.x, c.y, c.z ) ) )
        mBlocks[c.z][c.y][c.x]->toBreak();
    return true;
}
bool cUnderGround::isOutOfRange( const ci::ivec3& c )
{
    return  (uint) c.z < 0 || (uint) c.z > mBlocks.size() - 1 ||
        (uint) c.y < 0 || (uint) c.y > mBlocks[c.z].size() - 1 ||
        (uint) c.x < 0 || (uint) c.x > mBlocks[c.z][c.y].size() - 1;
}
ci::vec3 cUnderGround::getBlockCenterTopPosition( const ci::vec3 & target_position )
{
    auto c = getCellNumFromPosition( target_position );
    auto b = mBlocks[c.z][c.y][c.x];
    return b->mPosition + vec3( 0, b->mScale / 2, 0 );
}
ci::ivec3 cUnderGround::getBlockMaxCell()
{
    return mBlockMaxCell;
}
}
}
