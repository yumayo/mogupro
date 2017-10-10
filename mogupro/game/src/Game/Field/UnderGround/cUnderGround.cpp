#include <Game/Field/UnderGround/cUnderGround.h>

using namespace ci;

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
    int num = 16;
    float offset = 0.3f;
    float scale = 1;

    for (size_t z = 0; z < num; z++)
    {
        std::vector<std::vector<cBlock>> temps;
        for (size_t y = 0; y < num; y++)
        {
            std::vector<cBlock> temp;
            for (size_t x = 0; x < num; x++)
            {
                auto position = vec3(x * scale + x * offset,
                                     y * scale + y * offset,
                                     z * scale + z * offset);
                cBlock block(position, scale);

                // ‚Æ‚è‚ ‚¦‚¸’[‚Á‚±ˆÈŠO‚Ì•`‰æ‚ðØ‚é
                bool is_active = false;
                if (z == num - 1 || z == 0 ||
                    y == num - 1 || y == 0 ||
                    x == num - 1 || x == 0)
                    is_active = true;

                block.mIsActive = is_active;

                temp.emplace_back(block);
            }
            temps.emplace_back(temp);
            temp.clear();
        }
        blocks.emplace_back(temps);
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
                    draw_side.push_back(0);
                if (z == 0)
                    draw_side.push_back(1);

                if (x == blocks[z][y].size() - 1)
                    draw_side.push_back(3);
                if (x == 0)
                    draw_side.push_back(2);

                if (y == blocks[z].size() - 1)
                    draw_side.push_back(4);
                if (y == 0)
                    draw_side.push_back(5);

                blocks[z][y][x].setupDrawSide(&draw_side);
            }
        }
    }
}
void cUnderGround::update()
{
}
void cUnderGround::draw()
{
    for (size_t z = 0; z < blocks.size(); z++)
    {
        for (size_t y = 0; y < blocks[z].size(); y++)
        {
            for (size_t x = 0; x < blocks[z][y].size(); x++)
            {
                blocks[z][y][x].drawMesh();
            }
        }
    }
}
void cUnderGround::setupTestCube()
{
}
}
}
}