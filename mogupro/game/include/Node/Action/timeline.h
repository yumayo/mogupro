#pragma once
#include <Node/Action/finite_time_action.h>
namespace Node
{
namespace Action
{
class timeline : public finite_time_action
{
public:
    virtual ~timeline( ) { }
public:
    bool init( );
protected:
    std::vector<hardptr<finite_time_action>> _actions;
};
}
}