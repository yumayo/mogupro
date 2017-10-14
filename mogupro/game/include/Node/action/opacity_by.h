#pragma once
#include <Node/Action/opacity_to.h>
namespace Node
{
namespace Action
{
class opacity_by : public opacity_to
{
public:
    CREATE_H( opacity_by, float duration, float opacity );
    bool init( float duration, float opacity );
private:
    void setup( ) override;
    void restart( ) override;
private:
    float _init_opacity;
};
}
}