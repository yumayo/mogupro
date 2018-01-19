#pragma once
#include <Node/node.h>
namespace Game
{
namespace UI
{
class cItemSlot : public Node::node
{
public:
	CREATE_H( cItemSlot, cinder::vec2 contentSize )
	{
		CREATE( cItemSlot, contentSize );
	}
	bool init( cinder::vec2 contentSize );
	softptr<Node::node> capsules;
};
}
}
