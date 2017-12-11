#pragma once
#include <Node/Action/finite_time_action.h>
#include <cinder/Easing.h>
namespace Node
{
namespace Action
{
template<typename ease_type>
class ease : public finite_time_action
{
public:
    CREATE_H( ease<ease_type>, hardptr<finite_time_action> const& time_action );
    bool init( hardptr<finite_time_action> const& time_action );
public:
    virtual void setup( ) override;
    virtual bool is_done( ) override;
    virtual float update( float delta ) override;
    virtual void restart( ) override;
protected:
    ease_type ease_object;
    hardptr<finite_time_action> _time_action;
};

template<typename ease_type>
inline CREATE_CPP( ease<ease_type>, hardptr<finite_time_action> const & time_action )
{
    CREATE( ease<ease_type>, time_action );
}
template<typename ease_type>
inline bool ease<ease_type>::init( hardptr<finite_time_action> const & time_action )
{
    _time_action = time_action;
    return true;
}
template<typename ease_type>
inline void ease<ease_type>::setup( )
{
    _time_action->setup( _target, _pause );
}
template<typename ease_type>
inline bool ease<ease_type>::is_done( )
{
    return _time_action->is_done( );
}
template<typename ease_type>
inline float ease<ease_type>::update( float delta )
{
    auto overflow_second = _time_action->set_time( _time_action->get_time( ) + delta );
    _time_action->step( ease_object( _time_action->get_time( ) / _time_action->get_duration( ) ) );
    return overflow_second;
}
template<typename ease_type>
inline void ease<ease_type>::restart( )
{
    finite_time_action::restart( );
    _time_action->restart( );
}
}
}