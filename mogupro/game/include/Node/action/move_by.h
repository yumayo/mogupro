#pragma once
#include <Node/Action/move_to.h>
namespace Node
{
namespace Action
{
class move_by : public move_to
{
public:
    CREATE_H( move_by, float duration, cinder::vec2 position );
    bool init( float duration, cinder::vec2 position );
    CREATE_H( move_by, float duration, cinder::vec3 position );
    bool init( float duration, cinder::vec3 position );
private:
    void setup( ) override;
    void restart( ) override;
private:
    cinder::vec3 _init_position;
};
}
}