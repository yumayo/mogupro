#pragma once
#include <Node/Action/finite_time_action.h>
#include <cinder/Vector.h>
namespace Node
{
namespace Action
{
class scale_to : public finite_time_action
{
public:
    CREATE_H( scale_to, float duration, cinder::vec2 scale );
    bool init( float duration, cinder::vec2 scale );
    CREATE_H( scale_to, float duration, cinder::vec3 scale );
    bool init( float duration, cinder::vec3 scale );
protected:
    void setup( ) override;
    void step( float t ) override;
    void restart( ) override;
protected:
    cinder::vec3 _start_scale;
    cinder::vec3 _scale;
    bool _is_2d;
};
}
}