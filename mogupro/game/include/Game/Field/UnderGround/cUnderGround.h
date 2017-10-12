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

    bool blockBreak( const ci::vec3& position, const float& radius );
    bool isOutOfRange( const ci::ivec3& cell_num );

private:

    bool blockDigged( const  ci::ivec3& cell_num );

private:

    std::vector<std::vector<std::vector<std::shared_ptr<cBlock>>>> blocks;
    int num;
    float offset;
    float scale;
};
}
}
}



