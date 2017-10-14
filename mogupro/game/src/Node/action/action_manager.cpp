#include <Node/Action/action_manager.h>
#include <algorithm>
#include <Node/node.h>
namespace Node
{
namespace Action
{
void action_manager::add_action( hardptr<action> act, hardptr<node> target, bool pause )
{
    act->set_target( target );
    act->set_pause( pause );
    _actions.emplace_back( act );
}
softptr<action> action_manager::get_action_by_name( std::string const & name ) const
{
    // assert_log( !name.empty( ), "[action]無効な名前です。", return nullptr );

    std::hash<std::string> h;
    size_t hash = h( name );

    auto itr = std::find_if( std::cbegin( _actions ), std::cend( _actions ), [ this, hash, name ] ( hardptr<action> const& act )
    {
        return act->_hash == hash && act->_name.compare( name ) == 0;
    } );

    if ( itr != std::cend( _actions ) )
    {
        return *itr;
    }
    return nullptr;
}
softptr<action> action_manager::get_action_by_tag( int tag ) const
{
    // assert_log( tag == node::INVALID_TAG, "[action]無効なタグです。", return nullptr );

    auto itr = std::find_if( std::cbegin( _actions ), std::cend( _actions ), [ this, tag ] ( hardptr<action> const& act )
    {
        return act->_tag == tag;
    } );

    if ( itr != std::cend( _actions ) )
    {
        return *itr;
    }
    return nullptr;
}
void action_manager::remove_all_actions( )
{
    _actions.clear( );
}
void action_manager::remove_action( softptr<action> remove_act )
{
    if ( _actions.empty( ) ) return;
    auto erase_itr = std::remove_if( std::begin( _actions ), std::end( _actions ), [ this, remove_act ] ( hardptr<action>& act )
    {
        return act == remove_act;
    } );
    // action_managerのeraseはfor文の中で呼ばれないことが保証されています。
    _actions.erase( erase_itr, std::end( _actions ) );
}
void action_manager::remove_action_by_tag( int tag )
{
    // assert_log( tag == node::INVALID_TAG, "[action]無効なタグです。", return );
    if ( auto act = this->get_action_by_tag( tag ) )
    {
        remove_action( act );
    }
    else
    {
        // log( "[action]remove_action_by_tag( \"%d\" ): 子供が見つかりませんでした。", tag );
    }
}
void action_manager::remove_action_by_name( std::string const & name )
{
    // assert_log( !name.empty( ), "[action]無効な名前です。", return );
    if ( auto act = this->get_action_by_name( name ) )
    {
        remove_action( act );
    }
    else
    {
        // log( "[action]remove_action_by_name( \"%s\" ): 子供が見つかりませんでした。", name.c_str( ) );
    }
}
bool action_manager::is_running( ) const
{
    return !_actions.empty( );
}
void action_manager::update( float delta )
{
    for ( auto& act : _actions )
    {
        // managerまで溢れてくる値は無視します。delay::create(0.0F)とかあったら無限ループに入るので。
        act->update( delta );
    }
    auto erase = std::remove_if( std::begin( _actions ), std::end( _actions ), [ ] ( hardptr<action>& act )
    {
        return act->is_done( );
    } );
    _actions.erase( erase, std::end( _actions ) );
}
}
}