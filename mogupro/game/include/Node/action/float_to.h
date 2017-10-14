#pragma once
#include <Node/Action/finite_time_action.h>
namespace Node
{
namespace Action
{
class float_to : public finite_time_action
{
public:
    CREATE_H( float_to, float duration, float from, float to, std::function<void( float value )> callback );
    bool init( float duration, float from, float to, std::function<void( float value )> callback );
private:
    void step( float t ) override;
private:
    std::function<void( float value )> _callback;
    float _from = 0.0F;
    float _to = 0.0F;
};
}
}