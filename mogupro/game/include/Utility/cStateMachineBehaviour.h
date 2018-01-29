#pragma once
#include <string>
#include <functional>
#include <vector>
#include <boost/any.hpp>
#include <Utility/cUserPointer.hpp>
namespace Utility
{
class cStateNode;
class cStateAllow
{
public:
	cStateAllow( Utility::softptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> )> transitionInfo, std::function<boost::any( Utility::softptr<cStateNode> )> message );
	//! 次の遷移先
	Utility::softptr<cStateNode> nextNode;
	//! 返り値がtrueになったらステートが遷移します。
	std::function<bool( Utility::softptr<cStateNode> n )> transitionInfo;
	//! 遷移先に送るようのメッセージです。
	std::function<boost::any( Utility::softptr<cStateNode> )> message;
};
class cStateNode : public std::enable_shared_from_this<cStateNode>
{
public:
	//! このステートの名前
	std::string name;
	//! 遷移された瞬間に呼ばれます。
	std::function<void( boost::any message )> onStateIn;
	//! 遷移する瞬間に呼ばれます。
	std::function<void( )> onStateOut;
	//! 現在のステートが選択されていたら毎フレーム呼ばれます。
	std::function<void( Utility::softptr<cStateNode> self )> onStateStay;
	//! ノードが呼ばれ続ける限り時間が増えていきます。
	float time = 0.0F;
	//! ノードが遷移されるような状態であれば遷移先を返します。default: none
	Utility::softptr<cStateAllow> update( float delta );
	//! このノードに繋げる矢印を指定します。
	void join( Utility::hardptr<cStateNode> nextNode, std::function<bool( Utility::softptr<cStateNode> )> transitionInfo, std::function<boost::any( Utility::softptr<cStateNode> )> message = nullptr );
private:
	//! 遷移先の保持
	std::vector<Utility::hardptr<cStateAllow>> nextNodes;
};
class cStateMachineBehaviour
{
public:
    cStateMachineBehaviour( ) = default;
	~cStateMachineBehaviour( ) = default;
	Utility::softptr<cStateNode> generate( std::string const& name );
	#define STATE_GENERATE(stateMachine, name) auto name = stateMachine.generate( #name )
	void setEntryNode( Utility::softptr<cStateNode> entry );
	void update( float delta );
	bool isCurrentState( std::string const& stateName ) const;
	std::string const& getCurrentStateName( ) const;
private:
	Utility::softptr<cStateNode> entry;
	Utility::softptr<cStateNode> current;
	std::vector<Utility::hardptr<cStateNode>> nodes;
};
}
