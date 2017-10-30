#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>
namespace Scene
{
namespace Member
{
class cTitle final : public cSceneBase
{
    hardptr<Node::node> mRoot;
    int mSelectTag;
    std::vector<std::function<void( )>> mScenes;
public:
    cTitle( );
    ~cTitle( );
    void setup( );
    void shutDown( );
    void resize( );
    void update( float deltaTime );
    void draw( );
	void drawFBO();
    void draw2D( );
};
}
}
