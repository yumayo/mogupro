#pragma once
#include <Scene/cSceneBase.h>
#include <Game/SkyDome/cSkyDome.h>
#include <Node/node.h>
namespace Scene
{
namespace Member
{
class cGameMain : public cSceneBase
{
public:
    void setup( );
    void shutDown( );
    void update(float deltaTime);
    void draw( );
    void drawShadow( );
    void draw2D( );
    void resize( );
private:
    Game::SkyDome::cSkyDome skydome;
    hardptr<Node::node> n;
	bool sendEndSetup;
};
}
}
