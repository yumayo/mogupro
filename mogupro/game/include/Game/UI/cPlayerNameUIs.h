#pragma once
#include <Node/node.h>
namespace Game
{
namespace UI
{
class cPlayerNameUIs : public Node::node
{
public:
	CREATE_H( cPlayerNameUIs, int team )
	{
		CREATE( cPlayerNameUIs, team);
	}
	bool init( int team );
	void update( float delta ) override; 
private:
	int team;
};
}
}
