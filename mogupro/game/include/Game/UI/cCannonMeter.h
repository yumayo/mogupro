#pragma once
#include <Node/node.h>
#include <Game/Player/cPlayer.h>
namespace Game
{
namespace UI
{

class cCannonMeter : public Node::node
{
public:
	CREATE_H( cCannonMeter, cinder::vec2 contentSize, Player::Team playerTeam, Player::Team myTeam )
	{
		CREATE( cCannonMeter, contentSize, playerTeam, myTeam );
	}
	bool init( cinder::vec2 contentSize, Player::Team playerTeam, Player::Team myTeam );
	void addPower( int value );
	softptr<Node::node> meter;
	Player::Team playerTeam;
	Player::Team myTeam;
	softptr<Node::node> line;
	softptr<Node::node> player1;
	softptr<Node::node> player2;
	softptr<Node::node> player3;
	int power = 0;
};
}
}
