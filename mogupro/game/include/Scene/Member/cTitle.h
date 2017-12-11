#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>
#include <Utility/cFont.h>
namespace Scene
{
namespace Member
{
class cTitle final : public cSceneBase
{
	hardptr<Node::node> mBackGround;
	hardptr<Node::node> mContentsRoot;
public:
    cTitle( );
    ~cTitle( );
    void setup( );
    void shutDown( );
    void resize( );
    void update( float deltaTime );
    void draw( );
    void draw2D( );
};
}
}
