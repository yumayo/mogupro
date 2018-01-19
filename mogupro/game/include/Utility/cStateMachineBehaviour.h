#pragma once
#include <string>
#include <functional>
#include <set>
#include <Utility/cUserPointer.hpp>
namespace Utility
{
class cStateNode;
class cStateAllow
{
public:
	cStateAllow( Utility::hardptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> n )> transitionInfo );
	//! 次の遷移先
	Utility::hardptr<cStateNode> nextNode;
	//! 返り値がtrueになったらステートが遷移します。
	std::function<bool( Utility::softptr<cStateNode> n )> transitionInfo;
};
class cStateNode : public std::enable_shared_from_this<cStateNode>
{
public:
	//! 遷移先のnull扱い。
	static const Utility::hardptr<cStateNode> none;
	//! 現在のステートの名前
	std::string name;
	//! 遷移された瞬間に呼ばれます。
	std::function<void( Utility::softptr<cStateNode> prev )> onStateIn;
	//! 遷移する瞬間に呼ばれます。
	std::function<void( Utility::softptr<cStateNode> next )> onStateOut;
	//! 現在のステートが選択されていたら毎フレーム呼ばれます。
	std::function<void( Utility::softptr<cStateNode> self )> onStateStay;
	//! 遷移先の保持
	std::set<Utility::hardptr<cStateAllow>> nextNodes;
	//! ノードが遷移されるような状態であれば遷移先を返します。default: none
	Utility::softptr<cStateNode> update( );
	//! このノードに繋げるノードと矢印を指定します。
	void join( Utility::hardptr<cStateAllow> allow );
};
class cStateMachineBehaviour
{
public:
    cStateMachineBehaviour( ) = default;
    ~cStateMachineBehaviour( ) = default;
	void init( Utility::hardptr<cStateNode> entry );
	void update( );
private:
	Utility::hardptr<cStateNode> entry;
	Utility::softptr<cStateNode> current;
};
}
