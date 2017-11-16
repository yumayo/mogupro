#include <Game/Capsule/cQuarryCapsule.h>
namespace Game
{
namespace Capsule
{
cQuarryCapsule::cQuarryCapsule( )
{

}
cQuarryCapsule::~cQuarryCapsule( )
{

}
void cQuarryCapsule::setup(const ci::vec3 pos, const Game::Weapons::SubWeapon::SubWeaponType type)
{
	mPos = pos;
	mType = type;
}

void cQuarryCapsule::update(const float & delta_time)
{

}
void cQuarryCapsule::draw()
{

}
bool cQuarryCapsule::deleteThis()
{
	return false;
}
}
}
