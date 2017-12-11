#pragma once
#include <Node/Action/rotate_to.h>
namespace Node
{
namespace Action
{
class rotate_by : public rotate_to
{
public:
    CREATE_H( rotate_by, float duration, float radian );
    bool init( float duration, float radian );
private:
    void setup( ) override;
    void restart( ) override;
private:
    float _init_rotation = 0.0F;
};
}
}