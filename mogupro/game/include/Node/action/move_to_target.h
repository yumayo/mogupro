#pragma once
#include <Node/Action/finite_time_action.h>
#include <cinder/Vector.h>
namespace Node
{
namespace Action
{
class move_to_target : public finite_time_action
{
public:
    CREATE_H( move_to_target, float duration, hardptr<node> const& target );
    bool init( float duration, hardptr<node> const& target );
private:
    void setup( ) override;
    void step( float t ) override;
    void restart( ) override;
protected:
    cinder::vec3 _start_position;
    softptr<node> _new_target;
};
}
}