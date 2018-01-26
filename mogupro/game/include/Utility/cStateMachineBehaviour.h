#pragma once
#include <string>
#include <functional>
#include <set>
#include <vector>
#include <Utility/cUserPointer.hpp>
namespace Utility
{
class cStateNode : public std::enable_shared_from_this<cStateNode>
{
private:
	class cStateAllow
	{
	public:
		cStateAllow( Utility::softptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> )> transitionInfo );
		//! 次の遷移先
		Utility::softptr<cStateNode> nextNode;
		//! 返り値がtrueになったらステートが遷移します。
		std::function<bool( Utility::softptr<cStateNode> n )> transitionInfo;
	};
public:
	//! 遷移先のnull扱い。
	static const Utility::hardptr<cStateNode> none;
	//! このステートの名前
	std::string name;
	//! 遷移された瞬間に呼ばれます。
	std::function<void( Utility::softptr<cStateNode> prev )> onStateIn;
	//! 遷移する瞬間に呼ばれます。
	std::function<void( Utility::softptr<cStateNode> next )> onStateOut;
	//! 現在のステートが選択されていたら毎フレーム呼ばれます。
	std::function<void( Utility::softptr<cStateNode> self )> onStateStay;
	//! ノードが遷移されるような状態であれば遷移先を返します。default: none
	Utility::softptr<cStateNode> update( );
	//! このノードに繋げる矢印を指定します。
	void join( Utility::hardptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> )> transitionInfo );
private:
	//! 遷移先の保持
	std::set<Utility::hardptr<cStateAllow>> nextNodes;
};
class cStateMachineBehaviour
{
public:
    cStateMachineBehaviour( ) = default;
    ~cStateMachineBehaviour( ) = default;
	Utility::softptr<cStateNode> generate( std::string const& name = "" );
	void setEntryNode( Utility::softptr<cStateNode> entry );
	void update( );
	bool isCurrentState( std::string const& stateName ) const;
private:
	Utility::softptr<cStateNode> entry;
	Utility::softptr<cStateNode> current;
	std::vector<Utility::hardptr<cStateNode>> nodes;
};
}
