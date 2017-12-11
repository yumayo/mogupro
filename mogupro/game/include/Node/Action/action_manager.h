#pragma once
#include <Node/Action/action.h>
namespace Node
{
namespace Action
{
class action_manager
{
public:
    void add_action( hardptr<action> action, hardptr<node> target );
    softptr<action> get_action_by_name( std::string const& name )const;
    softptr<action> get_action_by_tag( int tag )const;
    void remove_all_actions( );
    void remove_action( softptr<action> remove_act );
    void remove_action_by_tag( int tag );
    void remove_action_by_name( std::string const& name );
    bool is_running( )const;
    void update( float delta );
private:
    std::vector<hardptr<action>> _actions;
};
}
}