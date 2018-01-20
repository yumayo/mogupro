#pragma once
#include <Node/node.h>
namespace Game
{
namespace UI
{
class cPlayerNameUIs : public Node::node
{
public:
	CREATE_H( cPlayerNameUIs )
	{
		CREATE( cPlayerNameUIs );
	}
	bool init( );
	void update( float delta ) override; 
};
}
}
