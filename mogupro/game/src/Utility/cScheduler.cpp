#include <Utility/cScheduler.h>
#include <Node/action.hpp>
using namespace Node::Action;
namespace Utility
{
cScheduler::cScheduler( )
	: mIdGen( 0, 100 )
{
	mRoot = Node::node::create( );
	mRoot->set_schedule_update( );
}
hardptr<int> cScheduler::apply( float updateSeconds, std::function<void( )> method, int loopNum )
{
	auto p = mHandles.insert(std::make_shared<int>(mIdGen.createId()));
	if ( loopNum == std::numeric_limits<int>::max( ) )
	{
		auto act = repeat_forever::create( sequence::create( delay::create( updateSeconds ), call_func::create( method ) ) );
		act->set_tag(**p.first);
		mRoot->run_action( act );
	}
	else
	{
		loopNum = glm::max( 1, loopNum );
		auto act = repeat_times::create( sequence::create( delay::create( updateSeconds ), call_func::create( method ) ), loopNum );
		act->set_tag(**p.first);
		mRoot->run_action( act );
	}
	return *p.first;
}
void cScheduler::applyLimitUpdate(float updateSeconds, std::function<void(float)> method, std::function<void(float)> finalizeMethod)
{
	mUpdateMethods.insert(std::make_pair(ci::app::getElapsedSeconds() + updateSeconds, LimitUpdateMethod{ method, finalizeMethod }));
}
void cScheduler::update( float delta )
{
	{
		auto it = mHandles.begin();
		while (it != mHandles.end())
		{
			if ((*it).use_count() == 1L)
			{
				mRoot->remove_action_by_tag(**it);
				mHandles.erase(it++);
			}
			else ++it;
		}
	}
	mRoot->entry_update( delta );

	{
		auto it = mUpdateMethods.begin();
		while (it != mUpdateMethods.end())
		{
			if (it->first < ci::app::getElapsedSeconds())
			{
				auto f = (*it).second.finalize;
				if (f) f(delta);
				mUpdateMethods.erase(it++);
			}
			else ++it;
		}
	}
	for (auto& method : mUpdateMethods) 
		method.second.update(delta);
}
}
