#pragma once
#include <cinder/app/App.h>
#include <Utility/cSingletonAble.h>
#include <Game/Field/cUnderGround.h>

namespace Game
{
class cFieldManager : public ::Utility::cSingletonAble<cFieldManager>
{
public:

    cFieldManager();
    ~cFieldManager();

    void setup();
    void update( const float delta_time );
    void draw();

    // position : ˆÊ’u
    // radius    : ‹…‚Ì”¼Œa
    void blockBreak( ci::vec3 position, float radius );

private:
    Field::cUnderGround mUnderGround;
};
}



