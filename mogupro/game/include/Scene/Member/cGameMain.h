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
	cinder::gl::GlslProgRef glsl;
    Game::SkyDome::cSkyDome skydome;
	bool sendEndSetup;

	bool endTimer;
	float gameStartTimer;
	boost::posix_time::ptime startTime;
	boost::posix_time::ptime endTime;
};
}
}
