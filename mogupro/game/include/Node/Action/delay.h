#pragma once
#include <Node/Action/finite_time_action.h>
namespace Node
{
namespace Action
{
class delay : public finite_time_action
{
public:
    CREATE_H( delay, float duration );

public:
    bool init( float duration );
};
}
}