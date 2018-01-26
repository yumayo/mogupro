#include <Utility/cStateMachineBehaviour.h>
namespace Utility
{
const Utility::hardptr<cStateNode> cStateNode::none;
cStateNode::cStateAllow::cStateAllow( Utility::softptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> )> transitionInfo )
	: nextNode( nextNode )
	, transitionInfo( transitionInfo )
{
}
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
void cStateNode::join( Utility::hardptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> )> transitionInfo )
{
	nextNodes.insert( std::make_shared<cStateAllow>( nextNode, transitionInfo ) );
}
Utility::softptr<cStateNode> cStateMachineBehaviour::generate( std::string const& name )
{
	auto itr = nodes.emplace( nodes.cend( ), std::make_shared<cStateNode>( ) );
	( *itr )->name = name;
	return *itr;
}
void cStateMachineBehaviour::setEntryNode( Utility::softptr<cStateNode> entry )
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
bool cStateMachineBehaviour::isCurrentState( std::string const& stateName ) const
{
	return current->name == stateName;
}
}
