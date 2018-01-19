#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>
namespace Scene
{
namespace Member
{
class cBegin final : public cSceneBase
{
public:
    cBegin( ) = default;
    ~cBegin( ) = default;
public:
	void setup( ) override;
	void shutDown( ) override;
	void update( float t ) override;
	void draw( ) override;
	void draw2D( ) override;
	void resize( ) override;
	std::string const& getName( ) { return typeid( *this ).name( ); }
private:
	hardptr<Node::node> root;
	softptr<Node::node> loadBar;
};
}
}
