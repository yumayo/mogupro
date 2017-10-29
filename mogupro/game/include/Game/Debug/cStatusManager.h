#pragma once
#include <Node/node.h>
namespace Game
{
namespace Debug
{
class cStatusManager
{
    hardptr<Node::node> mRoot;
public:
    void setup( );
    void update( );
    void draw2d( );
    void append( std::string const& line );
    cStatusManager( );
    ~cStatusManager( );
private:
};
}
}
