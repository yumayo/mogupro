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
	void setup( );
	void update( float delta );
	void draw( );
public:
	// 0.0 ~ 1.0
	void setRedCannonPower( float value );
	// 0.0 ~ 1.0
	void setBlueCannonPower( float value );
	// 
	void appendItem( int type );
	// 
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

	cinder::vec2 mAnimationSlot = cinder::vec2( 0.0F, 300.0F );
};
}
