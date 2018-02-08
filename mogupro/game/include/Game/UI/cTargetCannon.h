#pragma once
#include <Node/node.h>
#include <Game/Player/cPlayer.h>
namespace Game
{
namespace UI
{
class cTargetCannon : public Node::node
{
public:
	CREATE_H(cTargetCannon, Player::Team team);
	bool init(Player::Team team);
	void update(float delta) override;
private:
	softptr<Node::node> icon;
	softptr<Node::node> allow;
};
}
}
