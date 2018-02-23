#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>
#include <Utility/cStateMachineBehaviour.h>
namespace Scene
{
namespace Member
{
class cResult final : public cSceneBase
{
public:
    cResult( ) = default;
    ~cResult( ) = default;
public:
	void setup( ) override;
	void shutDown( ) override;
	void resize( ) override;
	void update( float deltaTime ) override;
	void draw( ) override;
	void draw2D( ) override;
	inline std::string const getName( ) { return typeid( *this ).name( ); }
private:
	Utility::cStateMachineBehaviour sMac;
	hardptr<Node::node> root3d;
	hardptr<Node::node> root;
	softptr<Node::node> rootUI;
	softptr<Node::node> winBoard, loseBoard;
	softptr<Node::node> eyeNode, tarNode, upNode;
	softptr<Node::node> redCapsuleNode, blueCapsuleNode;
	static hardptr<Node::node> createScoreBoard( int team, bool win, std::vector<std::string> playerNameData, std::vector<int> pointData, std::vector<int> killData, std::vector<int> deathData );
	static hardptr<Node::node> createPowerTorus( float time );
	static hardptr<Node::node> createScoreBar(std::string playerName, int pointData, int killData, int deathData);
};
}
}
