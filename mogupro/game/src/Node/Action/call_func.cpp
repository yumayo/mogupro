#include <Node/Action/finite_time_action.h>
#include <Node/node.h>
#include <Node/Action/call_func.h>
namespace Node
{
namespace Action
{
CREATE_CPP( call_func, std::function<void( )> call )
{
    CREATE( call_func, call );
}
bool call_func::init( std::function<void( )> call )
{
    _call = call;
    return true;
}
float call_func::update( float delta )
{
    if ( _call ) _call( );
    return delta;
}
}
}