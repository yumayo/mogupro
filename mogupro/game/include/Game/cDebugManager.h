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
public:
	cinder::params::InterfaceGlRef mParam;
public:
    void setup( );
	void update( float delta );
    void draw2d( );
};
}
