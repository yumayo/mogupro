#pragma once
#include "../cSceneBase.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include <Camera/cCameraManager.h>
#include <Utility/cInput.h>
#include <Game/SkyDome/cSkyDome.h>
#include <Game/cFieldManager.h>
#include <Game/cStrategyManager.h>
#include <Game/cGemManager.h>

using namespace ci;
using namespace ci::app;
using namespace std;

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
    float delta_time;
    ci::vec3 pos;
    ci::vec3 size;
    Game::SkyDome::cSkyDome skydome;
};
}
}
