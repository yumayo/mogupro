#pragma once
#include <Node/node.h>
#include <Game/Player/cPlayer.h>
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
	bool mHintTransportGem = false;
	bool mHintNearGemStone = false;
	bool mHintNearBlock    = false;
	softptr<Node::node> message;
};
}
}
