#include <Game/Field/cUnderGround.h>

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
    num = 8;
    offset = 0.0f;
    scale = 0.5f;

    for (int z = 0; z < num; z++)
    {
        std::vector<std::vector<std::shared_ptr<cBlock>>> temps;
        for (int y = 0; y < num; y++)
        {
            std::vector<std::shared_ptr<cBlock>> temp;
            for (int x = 0; x < num; x++)
            {
                auto position = vec3( x * scale + x * offset,
                                      y * scale + y * offset,
                                      z * scale + z * offset );
                std::shared_ptr<cBlock> block = std::make_shared<cBlock>( position, scale );

                // とりあえず端っこ以外の描画を切る
                //bool is_active = false;
                //if (z == num - 1 || z == 0 ||
                //     y == num - 1 || y == 0 ||
                //     x == num - 1 || x == 0)
                //    is_active = true;

                block->mIsActive = true;

                // メッシュを生成する方向を決める
                std::vector<int> draw_side;
                if (z == num - 1)
                    draw_side.push_back( 0 );
                if (z == 0)
                    draw_side.push_back( 1 );
                if (x == num - 1)
                    draw_side.push_back( 2 );
                if (x == 0)
                    draw_side.push_back( 3 );
                if (y == num - 1)
                    draw_side.push_back( 4 );
                if (y == 0)
                    draw_side.push_back( 5 );

                block->mDrawSide = draw_side;
                blockVertexBlend( block );

                temp.emplace_back( std::move( block ) );
            }
            temps.emplace_back( std::move( temp ) );
            temp.clear();
        }
        blocks.emplace_back( std::move( temps ) );
        temps.clear();
    }


    createMesh();
    blocks[3][0][0]->mIsActive = false;
    //blockDigged( ivec3( 0, 0, 3 ) );

    //blocks[4][num - 1][4]->mIsActive = false;
    //blockDigged( ivec3( 4, num - 1, 4 ) );

    //blocks[num - 1][4][4]->mIsActive = false;
    //blockDigged( ivec3( 4, 4, num - 1 ) );

}
void cUnderGround::update()
{
}
void cUnderGround::draw()
{
    /*for (size_t z = 0; z < blocks.size(); z++)
        for (size_t y = 0; y < blocks[z].size(); y++)
            for (size_t x = 0; x < blocks[z][y].size(); x++)
                blocks[z][y][x]->drawMesh();*/

    gl::draw( *mMesh );
}
bool cUnderGround::isOutOfRange( const ci::ivec3& c )
{
    return  (size_t) c.z < 0 || (size_t) c.z > blocks.size() - 1 ||
        (size_t) c.y < 0 || (size_t) c.y > blocks[c.z].size() - 1 ||
        (size_t) c.x < 0 || (size_t) c.x > blocks[c.z][c.y].size() - 1;
}
bool cUnderGround::blockDigged( const ci::ivec3& c )
{
    if (isOutOfRange( c ))
        return false;

    // とりあえず一からフィールドを作り直す
    blockClear();

    ivec3 dir[] = {
        ivec3( 0, 0, 1 ),
        ivec3( 0, 0,-1 ) ,
        ivec3( 1, 0, 0 ) ,
        ivec3( -1, 0, 0 ) ,
        ivec3( 0, 1, 0 ) ,
        ivec3( 0,-1, 0 ) };

    std::vector<ivec3> change_block;

    // 隣がないブロックを探す
    for (int i = 0; i < 6; i++)
    {
        auto cell = c + dir[i];
        if (isOutOfRange( cell ))
            continue;

        // 壊れたブロックの6方向を調べる
        auto b = blocks[cell.z][cell.y][cell.x];
        if (b->mIsActive == false)
            continue;

        std::vector<int> side;
        for (size_t k = 0; k < 6; k++)
        {
            auto search_cell = cell + dir[k];
            if (isOutOfRange( search_cell ))
            {
                side.push_back( k );
                continue;
            }
            if (blocks[search_cell.z][search_cell.y][search_cell.x]->mIsActive == false)
                side.push_back( k );
        }
        b->clear();
        b->mDrawSide = side;
        change_block.push_back( cell );
    }

    for (size_t z = 0; z < blocks.size(); z++)
        for (size_t y = 0; y < blocks[z].size(); y++)
            for (size_t x = 0; x < blocks[z][y].size(); x++)
            {
                blockVertexBlend( blocks[z][y][x],
                                  std::any_of( change_block.begin(), change_block.end(),
                                               [&]( ivec3 n ) { return n == ivec3( x, y, z ); } ) );
            }

    createMesh();
    return true;
}
void cUnderGround::blockVertexBlend( std::shared_ptr<cBlock> b, bool is_side_create )
{
    int max = 0;
    if (mIndices.size() > 0)
        max = *std::max_element( mIndices.begin(), mIndices.end() ) + 1;
    if (is_side_create)
        b->createSide( max );

    // vertices, indices, uvs, normals　を合わせる
    std::copy( b->mVertices.begin(), b->mVertices.end(), std::back_inserter( mVertices ) );
    std::copy( b->mIndices.begin(), b->mIndices.end(), std::back_inserter( mIndices ) );
    std::copy( b->mUv.begin(), b->mUv.end(), std::back_inserter( mUv ) );
    std::copy( b->mNormals.begin(), b->mNormals.end(), std::back_inserter( mNormals ) );
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
    return true;
}
void cUnderGround::blockClear()
{
    mVertices.clear();
    mIndices.clear();
    mUv.clear();
    mNormals.clear();
    mMesh->clear();
}
bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    auto p = ivec3( position / scale );

    if (isOutOfRange( p ))
        return false;
    blocks[p.z][p.y][p.x]->toBreak();
    blockDigged( p );

    auto r = ivec3( int( radius / scale ) );
    auto s = p - r;
    auto e = p + r;

    for (int z = s.z; z < e.z; z++)
        for (int y = s.y; y < e.y; y++)
            for (int x = s.x; x < e.x; x++)
            {
                if (isOutOfRange( ivec3( x, y, z ) ))
                    continue;
                blocks[z][y][x]->toBreak();
                blockDigged( ivec3( x, y, z ) );
            }
    return true;
}
}
}
