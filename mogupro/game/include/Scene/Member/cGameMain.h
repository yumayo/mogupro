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
    cinder::vec3 pos;
    cinder::vec3 size;
	//プレイヤー達の位置
	std::vector<ci::vec3> positions;
    Game::SkyDome::cSkyDome skydome;
    hardptr<Node::node> n;
};
}
}
