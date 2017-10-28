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
    ci::vec3 mCamPos;
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
    void draw2D( );
};
}
}
