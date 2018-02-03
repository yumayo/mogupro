#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/params/Params.h>
namespace Game
{
class cDebugManager : public Utility::cSingletonAble<cDebugManager>
{
private:
	float mFps;
	float mDelta;
	cinder::vec3 mPlayerPosition;
	int send_player;
	int resieve_player;
public:
	cinder::params::InterfaceGlRef mParam;
public:
    void setup( );
	void update( float delta );
    void draw2d( );
};
}
