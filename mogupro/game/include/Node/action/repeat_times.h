#pragma once
#include <Node/Action/repeat_forever.h>
namespace Node
{
namespace Action
{
class repeat_times : public repeat_forever
{
public:
    CREATE_H( repeat_times, hardptr<finite_time_action> const& time_action, int number_of_times );
public:
    bool init( hardptr<finite_time_action> const& time_action, int number_of_times );
public:
    virtual bool is_done( ) override;
    virtual float update( float delta ) override;
    virtual void restart( ) override;
private:
    int _init_number_of_times = 0;
    int _number_of_times = 0;
};
}
}