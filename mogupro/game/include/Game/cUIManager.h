#pragma once
#include <Node/node.h>
#include <Utility/cSingletonAble.h>
#include <boost/optional.hpp>
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
	void setRedCannonPower( int value );
	void setBlueCannonPower( int value );
	void setItem( boost::optional<int> currentItem, boost::optional<int> nextItem );
	void enable( );
	void disable( );
private:
	hardptr<Node::node> mRoot;
	softptr<Node::node> mTime;
	softptr<Node::node> mSlot;
	softptr<Node::node> mRedTeamCannonMeter;
	softptr<Node::node> mBlueTeamCannonMeter;
	softptr<Node::node> mPlayerScreenEffect;
};
}
