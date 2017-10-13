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
    void update( const int delta_time );
    void draw();

    // position : ˆÊ’u
    // radius    : ‹…‚Ì”¼Œa
    void blockBreak( ci::vec3 position, float radius );

private:
    UnderGround::cUnderGround mUnderGround;
};
}
}



