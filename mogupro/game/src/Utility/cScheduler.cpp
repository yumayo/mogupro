#include <Utility/cScheduler.h>
#include <Node/action.hpp>
namespace Utility
{
cScheduler::cScheduler( )
{
	mRoot = Node::node::create( );
	mRoot->set_schedule_update( );
}
hardptr<int> cScheduler::apply( float updateSeconds, std::function<void( )> method, int loopNum )
{
	int genId = ++mId;
	using namespace Node::Action;
	if ( loopNum == std::numeric_limits<int>::max( ) )
	{
		auto act = repeat_forever::create( sequence::create( delay::create( updateSeconds ), call_func::create( method ) ) );
		act->set_tag( genId );
		mRoot->run_action( act );
	}
	else
	{
		loopNum = glm::max( 1, loopNum );
		auto act = repeat_times::create( sequence::create( delay::create( updateSeconds ), call_func::create( method ) ), loopNum );
		act->set_tag( genId );
		mRoot->run_action( act );
	}
	return std::make_shared<int>( genId );
}
void cScheduler::update( float delta )
{
	auto it = mHandles.begin( );
	while ( it != mHandles.end( ) )
	{
		if ( (*it).use_count( ) == 1L )
		{
			mRoot->remove_action_by_tag( **it );
			mHandles.erase( it++ );
		}
		else ++it;
	}
	mRoot->entry_update( delta );
}
}
