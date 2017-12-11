#pragma once
#include <Node/Action/finite_time_action.h>
#include <cinder/Vector.h>
namespace Node
{
namespace Action
{
class opacity_to : public finite_time_action
{
public:
    CREATE_H( opacity_to, float duration, float opacity );
    bool init( float duration, float opacity );
protected:
    void setup( ) override;
    void step( float t ) override;
    void restart( ) override;
protected:
    float _start_opacity;
    float _opacity;
};
}
}