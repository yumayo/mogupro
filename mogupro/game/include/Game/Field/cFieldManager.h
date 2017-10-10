#pragma once
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

    void update();
    void draw();

private:
    UnderGround::cUnderGround mUnderGround;
};
}
}



