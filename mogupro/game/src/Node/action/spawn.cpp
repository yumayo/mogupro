#include <Node/Action/spawn.h>
#include <algorithm>
namespace Node
{
namespace Action
{
bool spawn::init( )
{
    return true;
}
void spawn::setup( )
{
    for ( auto& act : _actions )
    {
        act->setup( _target, _pause );
    }
}
bool spawn::is_done( )
{
    bool done = true;
    for ( auto const& act : _actions )
    {
        done = done && act->is_done( );
    }
    return done;
}
float spawn::update( float delta )
{
    if ( _actions.empty( ) ) return delta;
    std::vector<float> overflow_seconds;
    for ( auto const& act : _actions )
    {
        overflow_seconds.emplace_back( act->update( delta ) );
    }
    return *std::max_element( std::begin( overflow_seconds ), std::end( overflow_seconds ) );
}
void spawn::restart( )
{
    timeline::restart( );
    if ( is_done( ) )
    {
        for ( auto const& act : _actions )
        {
            act->restart( );
        }
        init( );
        setup( );
    }
}
}
}