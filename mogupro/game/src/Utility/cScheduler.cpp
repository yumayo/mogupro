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
hardptr<int> cScheduler::applyLimitUpdate(float updateSeconds, std::function<void(float)> method, std::function<void()> finalizeMethod)
{
	auto time = ci::app::getElapsedSeconds() + updateSeconds;
	auto it = mUpdateMethods.insert(std::make_pair(time, LimitUpdateMethod{ method, finalizeMethod }));
	auto p = mUpdateMethodHandles.insert(std::make_pair(std::make_shared<int>(mIdGen.createId()), time));
	if (!p.second) std::runtime_error( "時間が同じものは作れないンゴ。" );
	return p.first->first;
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

	// ハンドルが制御されなくなったら登録メソッドごと削除
	{
		auto it = mUpdateMethodHandles.begin();
		while (it != mUpdateMethodHandles.end())
		{
			if ((*it).first.use_count() == 1L)
			{
				auto targetSecond = (*it).second;
				auto& target = mUpdateMethods[targetSecond];
				auto f = target.finalize;
				if (f) f();
				mUpdateMethodHandles.erase(it++);
				mUpdateMethods.erase(targetSecond);
			}
			else ++it;
		}
	}
	{
		auto it = mUpdateMethods.begin();
		while (it != mUpdateMethods.end())
		{
			if ((*it).first < ci::app::getElapsedSeconds())
			{
				auto f = (*it).second.finalize;
				if (f) f();
				mUpdateMethods.erase(it++);
			}
			else
			{
				auto f = (*it).second.update;
				f(delta);
				++it;
			}
		}
	}
}
}
