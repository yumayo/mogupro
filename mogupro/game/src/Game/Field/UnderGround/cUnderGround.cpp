#include <Game/Field/UnderGround/cUnderGround.h>
#include "cinder/Timeline.h"

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
namespace UnderGround
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
    offset = 0.5f;
    scale = 0.5;

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
                bool is_active = false;
                if (z == num - 1 || z == 0 ||
                     y == num - 1 || y == 0 ||
                     x == num - 1 || x == 0)
                    is_active = true;

                block->mIsActive = true;

                temp.emplace_back( std::move( block ) );
            }
            temps.emplace_back( std::move( temp ) );
            temp.clear();
        }
        blocks.emplace_back( std::move( temps ) );
        temps.clear();
    }

    for (size_t z = 0; z < blocks.size(); z++)
    {
        for (size_t y = 0; y < blocks[z].size(); y++)
        {
            for (size_t x = 0; x < blocks[z][y].size(); x++)
            {
                std::vector<int> draw_side;
                if (z == blocks.size() - 1)
                    draw_side.push_back( 0 );
                if (z == 0)
                    draw_side.push_back( 1 );

                if (x == blocks[z][y].size() - 1)
                    draw_side.push_back( 2 );
                if (x == 0)
                    draw_side.push_back( 3 );

                if (y == blocks[z].size() - 1)
                    draw_side.push_back( 4 );
                if (y == 0)
                    draw_side.push_back( 5 );

                blocks[z][y][x]->setupDrawSide( draw_side );
            }
        }
    }

    blocks[3][0][0]->mIsActive = false;
    blockDigged( ivec3( 0, 0, 3 ) );

    blocks[4][num - 1][4]->mIsActive = false;
    blockDigged( ivec3( 4, num - 1, 4 ) );

    blocks[num - 1][4][4]->mIsActive = false;
    blockDigged( ivec3( 4, 4, num - 1 ) );
}
void cUnderGround::update()
{
}
void cUnderGround::draw()
{
    auto start = std::chrono::system_clock::now();

    for (size_t z = 0; z < blocks.size(); z++)
        for (size_t y = 0; y < blocks[z].size(); y++)
            for (size_t x = 0; x < blocks[z][y].size(); x++)
                blocks[z][y][x]->drawMesh();

    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    console() << msec << " milli sec" << std::endl;
}
bool cUnderGround::isOutOfRange( const ci::ivec3& c )
{
    return  c.z < 0 || c.z > blocks.size() - 1 ||
        c.y < 0 || c.y > blocks[c.z].size() - 1 ||
        c.x < 0 || c.x > blocks[c.z][c.y].size() - 1;
}
bool cUnderGround::blockDigged( const ci::ivec3& c )
{
    ivec3 dir[] = {
        ivec3( 0, 0, 1 ),
        ivec3( 0, 0,-1 ) ,
        ivec3( 1, 0, 0 ) ,
        ivec3( -1, 0, 0 ) ,
        ivec3( 0, 1, 0 ) ,
        ivec3( 0,-1, 0 ) };

    // 隣がないブロックを探す
    for (int i = 0; i < 6; i++)
    {
        auto cell = c + dir[i];
        if (isOutOfRange( cell ))
            continue;

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
            if (blocks[search_cell.z][search_cell.y][search_cell.x]
                 ->mIsActive == false)
                side.push_back( k );
        }
        b->clear();
        b->setupDrawSide( side );

    }
    return true;
}
bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    auto p = ivec3( position / scale );

    if (isOutOfRange( p ))
        return false;
    blocks[p.z][p.y][p.x]->toBreak();
    blockDigged( p );

    auto r = ivec3( radius / scale );
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
}