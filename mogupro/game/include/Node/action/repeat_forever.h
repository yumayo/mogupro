#pragma once
#include <Node/Action/finite_time_action.h>
namespace Node
{
namespace Action
{
class repeat_forever : public finite_time_action
{
public:
    CREATE_H( repeat_forever, hardptr<finite_time_action> const& );
public:
    bool init( hardptr<finite_time_action> const& time_action );
public:
    virtual void setup( ) override;
    virtual bool is_done( ) override;
    virtual float update( float delta ) override;
    virtual void restart( ) override;
protected:
    hardptr<finite_time_action> _time_action;
};
}
}