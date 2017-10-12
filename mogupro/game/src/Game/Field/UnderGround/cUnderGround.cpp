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
    num = 4;
    offset = 0.1f;
    scale = 0.5;

    for (int z = 0; z < num; z++)
    {
        std::vector<std::vector<cBlock>> temps;
        for (int y = 0; y < num; y++)
        {
            std::vector<cBlock> temp;
            for (int x = 0; x < num; x++)
            {
                auto position = vec3( x * scale + x * offset,
                                      y * scale + y * offset,
                                      z * scale + z * offset );
                cBlock block( position, scale );

                // ‚Æ‚è‚ ‚¦‚¸’[‚Á‚±ˆÈŠO‚Ì•`‰æ‚ðØ‚é
                bool is_active = false;
                if (z == num - 1 || z == 0 ||
                     y == num - 1 || y == 0 ||
                     x == num - 1 || x == 0)
                    is_active = true;

                block.mIsActive = is_active;

                temp.emplace_back( block );
            }
            temps.emplace_back( temp );
            temp.clear();
        }
        blocks.emplace_back( temps );
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
                    draw_side.push_back( 3 );
                if (x == 0)
                    draw_side.push_back( 2 );

                if (y == blocks[z].size() - 1)
                    draw_side.push_back( 4 );
                if (y == 0)
                    draw_side.push_back( 5 );

                blocks[z][y][x].setupDrawSide( &draw_side );
            }
        }
    }
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
                blocks[z][y][x].draw();

    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    console() << msec << " milli sec" << std::endl;
}
bool cUnderGround::isOutOfRange( ci::ivec3 c )
{
    return  c.z < 0 || c.z > blocks.size() - 1 ||
        c.y < 0 || c.y > blocks[c.z].size() - 1 ||
        c.x < 0 || c.x > blocks[c.z][c.y].size() - 1;
}
void cUnderGround::blockBreak( ci::vec3 position, float radius )
{
    auto p = ivec3( position / scale );

    if (isOutOfRange( p ))
        return;
    blocks[p.z][p.y][p.x].toBreak();

    auto r = ivec3( radius / scale );
    auto s = p - r;
    auto e = p + r;

    for (int z = s.z; z < e.z; z++)
        for (int y = s.y; y < e.y; y++)
            for (int x = s.x; x < e.x; x++)
            {
                if (isOutOfRange( ivec3( x, y, z ) ))
                    continue;
                blocks[z][y][x].toBreak();
            }
}
}
}
}