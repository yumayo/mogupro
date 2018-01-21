#pragma once
#include <Scene/cSceneBase.h>
#include <Game/SkyDome/cSkyDome.h>
#include <Node/node.h>
#include <cinder/gl/GlslProg.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>
#include <Collision/cCollisionManager.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
namespace Scene
{
namespace Member
{
class cGameMain : public cSceneBase
{
public:
	cGameMain( );
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
	Collision::cAABBCollider collider;
	Collision::cRigidBody rigid;
};
}
}
