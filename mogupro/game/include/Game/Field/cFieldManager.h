#pragma once
#include <cinder/app/App.h>
#include <Utility/cSingletonAble.h>
#include <Game/Field/UnderGround/cUnderGround.h>

namespace Game
{
namespace Field
{
class cFieldManager : public Utility::cSingletonAble<cFieldManager>
{
public:

    cFieldManager();
    ~cFieldManager();

    void setup();
    void update(const int delta_time);
    void draw();

    void blockBreak(ci::ivec3 block_num);
    void blockBreak(int x, int y, int z);

private:
    UnderGround::cUnderGround mUnderGround;
};
}
}



