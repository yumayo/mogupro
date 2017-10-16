#include <Game/Field/cUnderGround.h>
#include <Resource/TextureManager.h>
#include <Utility/cTimeMeasurement.h>

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

    mNum = 32;
    mHeight = 8;
    mIntervalOffset = 0.0f;
    mScale = 1.0f;
    mOffset = vec3( mScale / 2 );

    uint count = 0;
    for (int z = 0; z < mNum; z++)
    {
        std::vector<std::vector<std::shared_ptr<cBlock>>> temps;
        for (int y = 0; y < mHeight; y++)
        {
            std::vector<std::shared_ptr<cBlock>> temp;
            for (int x = 0; x < mNum; x++)
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


    createMesh();
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
bool cUnderGround::isOutOfRange( const ci::ivec3& c )
{
    return  (uint) c.z < 0 || (uint) c.z > blocks.size() - 1 ||
        (uint) c.y < 0 || (uint) c.y > blocks[c.z].size() - 1 ||
        (uint) c.x < 0 || (uint) c.x > blocks[c.z][c.y].size() - 1;
}
bool cUnderGround::blockDigged( const ci::ivec3& c )
{
    if (isOutOfRange( c ))
        return false;

    auto b = blocks[c.z][c.y][c.x];
    if (b->mIsActive == false)
        return false;

    cTimeMeasurement::getInstance()->make();
    blockMeshErase( b );

    //ivec3 dir[] = {
    //    ivec3( 0, 0, 1 ),
    //    ivec3( 0, 0,-1 ) ,
    //    ivec3( 1, 0, 0 ) ,
    //    ivec3( -1, 0, 0 ) ,
    //    ivec3( 0, 1, 0 ) ,
    //    ivec3( 0,-1, 0 ) };

    //std::vector<ivec3> change_block;
    // 消したブロックの周囲6マスのブロックのメッシュを作り直す
    //for (int i = 0; i < 6; i++)
    //{
    //    auto cell = c + dir[i];
    //    if (isOutOfRange( cell ))
    //        continue;

    //    // ブロックが存在していなかったらはじく
    //    auto b = blocks[cell.z][cell.y][cell.x];
    //    if (b->mIsActive == false)
    //        continue;

    //    std::vector<int> side;
    //    for (int k = 0; k < 6; k++)
    //    {
    //        auto search_cell = cell + dir[k];
    //        if (isOutOfRange( search_cell ))
    //            side.push_back( k );
    //        else if (blocks[search_cell.z][search_cell.y][search_cell.x]->mIsActive == false)
    //            side.push_back( k );
    //    }

    //    b->clear();
    //    b->mDrawSide = side;
    //    // メッシュに変更があったブロックを登録
    //    change_block.push_back( cell );
    //}

    //createMesh();
    cTimeMeasurement::getInstance()->make();
    console() << "msec : " << cTimeMeasurement::getInstance()->deltaTime() << std::endl;
    return true;
}
void cUnderGround::blockMeshBlend( std::shared_ptr<cBlock> b )
{
    b->createSide( mVertices, mIndices, mUv, mNormals );
}
void cUnderGround::blockMeshErase( std::shared_ptr<cBlock> b )
{
    uint indices[] = {
        0,0,0,0,0,0 ,
        0,0,0,0,0,0 ,
        0,0,0,0,0,0 ,
        0,0,0,0,0,0 ,
        0,0,0,0,0,0 ,
        0,0,0,0,0,0 , };
    for (size_t i = 0; i < b->mIndicesNum.size(); i++)
    {
        auto k = b->mIndicesNum[i];
        mIndices[k] = 0;
    }
    auto vbo = mVboMesh->getIndexVbo();
    vbo->bufferSubData( b->mIndicesNum[0] * 4, b->mIndicesNum.size() * 4, &indices[0] );

}
bool cUnderGround::createMesh()
{
    auto mMesh = ci::TriMesh::create();
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
void cUnderGround::blockClear()
{
    mVertices.clear();
    mIndices.clear();
    mUv.clear();
    mNormals.clear();
}
bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    auto p = ivec3( (position + mOffset) / mScale ) * ivec3( 1, -1, 1 );

    if (isOutOfRange( p ))
        return false;

    auto r = ivec3( int( radius / mScale ) );
    auto s = p - r;
    auto e = p + r;

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
}
}
