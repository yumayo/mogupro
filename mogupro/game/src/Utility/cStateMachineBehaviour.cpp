#include <Utility/cStateMachineBehaviour.h>
#include <boost/none.hpp>
namespace Utility
{
cStateAllow::cStateAllow( Utility::softptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> )> transitionInfo, std::function<boost::any( Utility::softptr<cStateNode> )> message )
	: nextNode( nextNode )
	, transitionInfo( transitionInfo )
	, message( message )
{
}
Utility::softptr<cStateAllow> cStateNode::update( float delta )
{
	for ( auto s : nextNodes )
	{
		if ( s->transitionInfo( shared_from_this( ) ) )
		{
			return s;
		}
	}
	return nullptr;
}
void cStateNode::join( Utility::hardptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> )> transitionInfo, std::function<boost::any( Utility::softptr<cStateNode> )> message )
{
	nextNodes.emplace_back( std::make_shared<cStateAllow>( nextNode, transitionInfo, message ) );
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
	if ( current->onStateIn ) current->onStateIn( boost::none );
}
void cStateMachineBehaviour::update( float delta )
{
	current->time += delta;
	if( current->onStateStay ) current->onStateStay( current );
	auto const allow = current->update( delta );
	if ( allow )
	{
		if ( current->onStateOut ) current->onStateOut( );
		current->time = 0.0F;
		auto prevNode = current;
		current = allow->nextNode;
		if ( current->onStateIn ) current->onStateIn( allow->message ? allow->message( prevNode ) : boost::none );
	}
}
bool cStateMachineBehaviour::isCurrentState( std::string const& stateName ) const
{
	return current->name == stateName;
}
std::string const & cStateMachineBehaviour::getCurrentStateName( ) const
{
	return current->name;
}
}
