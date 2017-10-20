#pragma once
#include <Node/node.h>
namespace Node
{
namespace Renderer
{
class circle : public node
{
public:
    CREATE_H( circle, float radius, int segments = -1 );
    virtual ~circle( );
    bool init( float radius, int segments = -1 );
    virtual void render( ) override;
public:
    void set_radius( float value );
    float get_radius( );
    void set_segments( int value );
    int get_segments( );
protected:
    float _radius = 0.0F;
    int _segments = -1;
};
}
}