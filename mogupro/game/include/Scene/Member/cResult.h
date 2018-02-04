#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>
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
	hardptr<Node::node> root;
	hardptr<Node::node> createScoreBoard( int team, bool win, std::vector<std::string> playerNameData, std::vector<int> pointData, std::vector<int> killData, std::vector<int> deathData );
};
}
}
