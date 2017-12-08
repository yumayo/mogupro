#pragma once
#include <Node/node.h>
#include <Utility/cSingletonAble.h>
namespace Game
{
class cUIManager : public Utility::cSingletonAble<cUIManager>
{
public:
    cUIManager( ) = default;
    ~cUIManager( ) = default;
public:
	void awake( );
	void setup( );
	void update( float delta );
	void draw( );
public:
	void addRedCannonPower( int value );
	void addBlueCannonPower( int value );
	void appendItem( int type );
	int winTeam( );
	cinder::ivec2 result( ) { return cinder::ivec2( redCannonPower, blueCannonPower ); }
	void useItem( );

	void enable( );
	void disable( );
private:
	hardptr<Node::node> mRoot;
	softptr<Node::node> mTime;
	softptr<Node::node> mLive;
	softptr<Node::node> mSlot;
	softptr<Node::node> mCapsule;
	softptr<Node::node> mRedTeamCannonPower;
	softptr<Node::node> mBlueTeamCannonPower;
	softptr<Node::node> mPlayerScreenEffect;

	cinder::vec2 mDisableSlot;
	cinder::vec2 mEnableSlot;

	int redCannonPower;
	int blueCannonPower;
};
}
