#include <Node/Action/action.h>
#include <Node/node.h>
namespace Node
{
namespace Action
{
CREATE_CPP( action )
{
    CREATE( action );
}
bool action::init( )
{
    return true;
}
void action::setup( hardptr<node> const& target, bool pause )
{
    _target = target;
    _pause = pause;
    setup( );
}
float action::update( float delta )
{
    return 0.0F;
}
bool action::is_done( )
{
    return true;
}
float action::ease_liner( float t, float from, float to )
{
    return ( to - from ) * t + from;
}
void action::setup( )
{
}
void action::step( float t )
{
}
void action::set_target( hardptr<node> const& value )
{
    _target = value;
}
softptr<node> action::get_target( )
{
    return _target;
}
void action::set_tag( int value )
{
    _tag = value;
}
int action::get_tag( )
{
    return _tag;
}
void action::set_name( std::string const & value )
{
    _name = value;

    std::hash<std::string> h;
    _hash = h( value );
}
std::string action::get_name( )
{
    return _name;
}
void action::set_pause( bool value )
{
    _pause = value;
}
bool action::get_pause( )
{
    return _pause;
}
}
}