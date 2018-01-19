#include <Utility/cStateMachineBehaviour.h>
namespace Utility
{
cStateAllow::cStateAllow( Utility::hardptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode>n )> transitionInfo )
	: nextNode( nextNode )
	, transitionInfo( transitionInfo )
{
}
const Utility::hardptr<cStateNode> cStateNode::none;
Utility::softptr<cStateNode> cStateNode::update( )
{
	for ( auto s : nextNodes )
	{
		if ( s->transitionInfo( shared_from_this( ) ) )
		{
			return s->nextNode;
		}
	}
	return cStateNode::none;
}
void cStateNode::join( Utility::hardptr<cStateAllow> allow )
{
	nextNodes.insert( allow );
}
void cStateMachineBehaviour::init( Utility::hardptr<cStateNode> entry )
{
	this->entry = entry;
	this->current = entry;
}
void cStateMachineBehaviour::update( )
{
	if( current->onStateStay ) current->onStateStay( current );
	auto const node = current->update( );
	if ( node != cStateNode::none )
	{
		if ( current->onStateOut ) current->onStateOut( node );
		current = node;
		if ( current->onStateIn ) current->onStateIn( node );
	}
}
}
