#pragma once
#include <Node/Action/finite_time_action.h>
#include <cinder/Vector.h>
namespace Node
{
namespace Action
{
class move_to : public finite_time_action
{
public:
    CREATE_H( move_to, float duration, cinder::vec2 position );
    bool init( float duration, cinder::vec2 position );
    CREATE_H( move_to, float duration, cinder::vec3 position_3d );
    bool init( float duration, cinder::vec3 position_3d );
protected:
    void setup( ) override;
    void step( float t ) override;
    void restart( ) override;
protected:
    cinder::vec3 _start_position;
    cinder::vec3 _position;
    bool _is_2d;
};
}
}