#include <Game/Weapons/SubWeapon/cLightBomb.h>
#include"Game\cPlayerManager.h"
namespace Game
{
namespace Weapons
{
	namespace SubWeapon {
		cLightBomb::cLightBomb(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 speed, const int playerid)
		{
		}
		cLightBomb::~cLightBomb()
		{

		}
		void cLightBomb::setup()
		{
			ci::app::console() << "‚¤‚Ü‚ê‚½" << std::endl;
		}
		void cLightBomb::update(const float & delta_time)
		{

		}
		void cLightBomb::updateCollisionAfterUpdate(const float & delta_time)
		{

		}
		void cLightBomb::draw()
		{

		}
		bool cLightBomb::deleteThis()
		{
			return false;
		}
	}
}
}