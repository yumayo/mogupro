#include <Node/Action/move_by.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( move_by, float duration, cinder::vec2 position )
{
    CREATE( move_by, duration, position );
}
bool move_by::init( float duration, cinder::vec2 position )
{
    move_to::init( duration, position );
    _init_position = vec3( position, 0.0F );
    return true;
}
CREATE_CPP( move_by, float duration, cinder::vec3 position )
{
    CREATE( move_by, duration, position );
}
bool move_by::init( float duration, cinder::vec3 position )
{
    move_to::init( duration, position );
    _init_position = position;
    return true;
}
void move_by::setup( )
{
    move_to::setup( );
    _position = _init_position + _start_position;
}
void move_by::restart( )
{
    move_to::restart( );
    setup( );
}
}
}