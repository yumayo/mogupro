#pragma once
#include <Scene/cSceneBase.h>
#include <Game/SkyDome/cSkyDome.h>
#include <Node/node.h>
#include <cinder/gl/GlslProg.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>

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
	bool sendEndSetup;
	bool endTimer;
};
}
}
