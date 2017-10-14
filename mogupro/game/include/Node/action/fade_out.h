#pragma once
#include <Node/Action/finite_time_action.h>
namespace Node
{
namespace Action
{
class fade_out : public finite_time_action
{
public:
    CREATE_H( fade_out, float duration );
    bool init( float duration );
private:
    void setup( ) override;
    void step( float t ) override;
private:
    float _opacity = 0.0F;
};
}
}