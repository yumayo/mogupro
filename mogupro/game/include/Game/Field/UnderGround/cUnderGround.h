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

private:
    std::vector<std::vector<std::vector<cBlock>>> blocks;
};
}
}
}



