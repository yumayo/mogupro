#pragma once
#include <Node/node.h>
#include <Game/Player/cPlayer.h>
#include <Utility/cStateMachineBehaviour.h>
namespace Game
{
namespace UI
{
class cTips : public Node::node
{
public:
	CREATE_H( cTips, cinder::vec2 baseContentSize, Player::Team team )
	{
		CREATE( cTips, baseContentSize, team );
	}
	bool init( cinder::vec2 baseContentSize, Player::Team team );
	void update( float delta ) override;
private:
	bool mYokuyatta = false;
	softptr<Node::node> message;
	softptr<Node::node> attack;
	softptr<Node::node> dig;
	softptr<Node::node> block;
	softptr<Node::node> gem;
	Utility::cStateMachineBehaviour mStateMachine;
};
}
}
