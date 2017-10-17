#include <Node/Action/finite_time_action.h>
#include <Node/node.h>
#include <Node/Action/delay.h>
namespace Node
{
namespace Action
{
CREATE_CPP( delay, float duration )
{
    CREATE( delay, duration );
}
bool delay::init( float duration )
{
    finite_time_action::init( duration );
    return true;
}
}
}