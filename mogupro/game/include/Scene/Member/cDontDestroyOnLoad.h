#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>
namespace Scene
{
namespace Member
{
class cDontDestroyOnLoad final : public cSceneBase
{
public:
    cDontDestroyOnLoad( ) = default;
    ~cDontDestroyOnLoad( ) = default;
public:
	void setup( ) override;
	void shutDown( ) override;
	void update( float t ) override;
	void draw( ) override;
	void draw2D( ) override;
	void resize( ) override;
	inline std::string const getName( ) { return typeid( *this ).name( ); }
public:
	hardptr<Node::node> root;
};
}
}
