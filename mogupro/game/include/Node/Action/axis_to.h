#pragma once
#include <Node/Action/finite_time_action.h>
#include <cinder/Vector.h>
namespace Node
{
namespace Action
{
class axis_to : public finite_time_action
{
public:
    CREATE_H( axis_to, float duration, cinder::vec3 axis );
    bool init( float duration, cinder::vec3 axis );
protected:
    void setup( ) override;
    void step( float t ) override;
    void restart( ) override;
protected:
    cinder::vec3 _start_axis = cinder::vec3( 0, 0, 1 );
    cinder::vec3 _axis = cinder::vec3( 0, 0, 1 );
};
}
}