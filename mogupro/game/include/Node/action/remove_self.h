#pragma once
#include <Node/Action/finite_time_action.h>
namespace Node
{
namespace Action
{
class remove_self : public finite_time_action
{
public:
    CREATE_H( remove_self );

public:
    bool init( );
public:
    float update( float delta ) override;
};
}
}