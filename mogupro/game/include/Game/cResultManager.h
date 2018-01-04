#pragma once
#include <Utility/cSingletonAble.h>
#include <Node/node.h>
namespace Game
{
class cResultManager : public Utility::cSingletonAble<cResultManager>
{
public:
    cResultManager( );
    ~cResultManager( );
public:
	void setup( );
	void update( float t );
	void draw( );
private:
	hardptr<Node::node> root;
};
}
