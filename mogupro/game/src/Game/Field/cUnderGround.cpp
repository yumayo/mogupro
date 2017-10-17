#include <Game/Field/cUnderGround.h>
#include <Resource/TextureManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Network/cUDPManager.h>
#include <Network/cRequestManager.h>

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

    mBlockMaxCell = ivec3( 16, 4, 16 );
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
    for (int z = 0; z < mBlockMaxCell.z; z++)
    {
        std::vector<std::vector<std::shared_ptr<cBlock>>> temps;
        for (int y = 0; y < mBlockMaxCell.y; y++)
        {
            std::vector<std::shared_ptr<cBlock>> temp;
            for (int x = 0; x < mBlockMaxCell.x; x++)
            {
                auto position = vec3( x * mScale + x * mIntervalOffset,
                                      y * -mScale + y * mIntervalOffset,
                                      z * mScale + z * mIntervalOffset ) + mOffset;
                std::shared_ptr<cBlock> block = std::make_shared<cBlock>( position, mScale, count );

                block->mDrawSide = { 0,1,2,3,4,5 };
                blockMeshBlend( block );

                temp.emplace_back( std::move( block ) );
            }
            temps.emplace_back( std::move( temp ) );
            temp.clear();
        }
        blocks.emplace_back( std::move( temps ) );
        temps.clear();
    }
    cTimeMeasurement::getInstance()->make();

    console() << std::endl << std::endl;
    console() << "Field create time : " << cTimeMeasurement::getInstance()->deltaTime() << " sec" << std::endl;
    console() << std::endl << std::endl;

    createMesh();
    return false;
}
bool cUnderGround::createMesh()
{
    mMesh = ci::TriMesh::create();
    if (mVertices.size() > 0)
        mMesh->appendPositions( &mVertices[0], mVertices.size() );
    if (mIndices.size() > 0)
        mMesh->appendIndices( &mIndices[0], mIndices.size() );
    if (mUv.size() > 0)
        mMesh->appendTexCoords0( &mUv[0], mUv.size() );
    if (mNormals.size() > 0)
        mMesh->appendNormals( &mNormals[0], mNormals.size() );
    mVboMesh = gl::VboMesh::create( *mMesh );
    return true;
}
void cUnderGround::blockMeshBlend( std::shared_ptr<cBlock> b )
{
    b->createSide( mVertices, mIndices, mUv, mNormals );
}
void cUnderGround::blockMeshErase( std::shared_ptr<cBlock> b )
{
    auto indices = std::vector<uint>( b->mIndicesNum.size() * 4 );
    for (size_t i = 0; i < b->mIndicesNum.size(); i++)
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
    if (isOutOfRange( c ))
        return false;

    auto b = blocks[c.z][c.y][c.x];
    if (b->mIsActive == false)
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
    return ivec3( (position + mOffset) / mScale ) * ivec3( 1, -1, 1 );
}
bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    auto c = getCellNumFromPosition( position );

    if (isOutOfRange( c ))
        return false;

    auto r = ivec3( int( radius / mScale ) );
    auto s = c - r;
    auto e = c + r;

    for (int z = s.z; z < e.z; z++)
        for (int y = s.y; y < e.y; y++)
            for (int x = s.x; x < e.x; x++)
            {
                if (isOutOfRange( ivec3( x, y, z ) ))
                    continue;
                if (blockDigged( ivec3( x, y, z ) ))
                    blocks[z][y][x]->toBreak();
            }
    return true;
}
bool cUnderGround::blockBreak( const ci::ivec3 & cell_num, const float & radius )
{
    auto c = cell_num;
    if (isOutOfRange( cell_num ))
        return false;
    if (blockDigged( ivec3( c.x, c.y, c.z ) ))
        blocks[c.z][c.y][c.x]->toBreak();
    return true;
}
bool cUnderGround::isOutOfRange( const ci::ivec3& c )
{
    return  (uint) c.z < 0 || (uint) c.z > blocks.size() - 1 ||
        (uint) c.y < 0 || (uint) c.y > blocks[c.z].size() - 1 ||
        (uint) c.x < 0 || (uint) c.x > blocks[c.z][c.y].size() - 1;
}
ci::vec3 cUnderGround::getBlockCenterTopPosition( const ci::vec3 & target_position )
{
    auto c = getCellNumFromPosition( target_position );
    auto b = blocks[c.z][c.y][c.x];
    return b->mPosition + vec3( 0, b->mScale / 2, 0 );
}
ci::ivec3 cUnderGround::getBlockMaxCell()
{
    return mBlockMaxCell;
}
}
}
