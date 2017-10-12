#pragma once
#include <Game/Field/UnderGround/cBlock.h>

namespace Game
{
namespace Field
{
namespace UnderGround
{
class cUnderGround
{
public:

    cUnderGround();
    ~cUnderGround();

    void setup();
    void update();
    void draw();

public:

    void blockBreak( ci::vec3 position, float radius );
    bool isOutOfRange( ci::ivec3 cell_num );

private:

private:
    std::vector<std::vector<std::vector<cBlock>>> blocks;

private:
    int num;
    float offset;
    float scale;
};
}
}
}



