#pragma once
#include <Scene/cSceneBase.h>
#include <Game/SkyDome/cSkyDome.h>
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
    void draw2D( );
    void resize( );
private:
    cinder::vec3 pos;
    cinder::vec3 size;
    Game::SkyDome::cSkyDome skydome;
};
}
}
