#include <Node/Action/sequence.h>
namespace Node
{
namespace Action
{
bool sequence::init( )
{
    _target_action = _actions.begin( );
    return true;
}
void sequence::setup( )
{
    if ( is_done( ) ) return;
    ( *_target_action )->setup( _target, _pause );
}
bool sequence::is_done( )
{
    return _target_action == _actions.end( );
}
float sequence::update( float delta )
{
    for ( ;
          !( delta == 0.0F || is_done( ) );
          ++_target_action, setup( ) )
    {
        delta = ( *_target_action )->update( delta );
        if ( !( *_target_action )->is_done( ) ) return 0.0F;
    }
    return delta;
}
void sequence::restart( )
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