#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Timeline.h>
#include <Node/node.h>
#include <limits>
#include <set>
namespace Utility
{
class cScheduler : public Utility::cSingletonAble<cScheduler>
{
public:
    cScheduler( );
    ~cScheduler( ) = default;
public:
	// 何秒おきに実行したいのか
	// 実行したいメソッド
	// 呼んで欲しい回数 default:無限に繰り返し
	hardptr<int> apply( float updateSeconds, std::function<void()> method, int callNum = std::numeric_limits<int>::max( ) );
	void update( float delta );
private:
	hardptr<Node::node> mRoot;
	std::set<hardptr<int>> mHandles;
	int mId = 0;
};
}
