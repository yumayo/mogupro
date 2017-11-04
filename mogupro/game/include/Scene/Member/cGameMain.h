#pragma once
#include <Scene/cSceneBase.h>
#include <Game/SkyDome/cSkyDome.h>
#include <Node/node.h>
#include <cinder/gl/GlslProg.h>
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
	cinder::gl::GlslProgRef glsl;
    Game::SkyDome::cSkyDome skydome;
    hardptr<Node::node> n;
	bool sendEndSetup;
};
}
}
