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

    void setupTestCube();

    // position : target position
    // range    : break range
    void blockBreak(ci::vec3 position, int range);

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



