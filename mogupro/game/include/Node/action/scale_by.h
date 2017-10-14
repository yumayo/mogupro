#pragma once
#include <Node/Action/scale_to.h>
namespace Node
{
namespace Action
{
class scale_by : public scale_to
{
public:
    CREATE_H( scale_by, float duration, cinder::vec2 scale );
    bool init( float duration, cinder::vec2 scale );
    CREATE_H( scale_by, float duration, cinder::vec3 scale );
    bool init( float duration, cinder::vec3 scale );
private:
    void setup( ) override;
    void restart( ) override;
private:
    cinder::vec3 _init_scale;
};
}
}