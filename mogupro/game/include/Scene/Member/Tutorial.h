#pragma once
#include <Scene/cSceneBase.h>
#include <Game/SkyDome/cSkyDome.h>
#include <Node/node.h>
#include <cinder/gl/GlslProg.h>


namespace Scene
{
namespace Member
{
class cTutorial : public cSceneBase
{
public:
    void setup( );
    void shutDown( );
    void update(float deltaTime);
    void draw( );
    void drawShadow( );
    void draw2D( );
    void resize( );
	std::string const& getName( ) { return typeid( *this ).name( ); }
private:
    Game::SkyDome::cSkyDome skydome;
	bool sendEndSetup;
};
}
}
