#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>
#include <Utility/cFont.h>
#include <Sound/cIntroLoopableBGM.h>
#include <Utility/cStateMachineBehaviour.h>
namespace User
{
class SceneTitle;
}
namespace Scene
{
namespace Member
{
class cTitle final : public cSceneBase
{
	Utility::hardptr<Node::node> root;
	Utility::softptr<Node::node> fader;
	Sound::cIntroLoopableBGM introloopBGM;
	Utility::cStateMachineBehaviour sMac;
	Utility::softptr<Utility::cStateNode> fadeout;
	std::shared_ptr<::User::SceneTitle> kurikoTitle;
public:
    cTitle( );
    ~cTitle( );
    void setup( );
    void shutDown( );
    void resize( );
    void update( float deltaTime );
    void draw( );
    void draw2D( );
	inline std::string const getName( ) { return typeid( *this ).name( ); }
};
}
}
