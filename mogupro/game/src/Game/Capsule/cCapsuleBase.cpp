#include <Game/Capsule/cCapsuleBase.h>
namespace Game
{
namespace Capsule
{
cCapsuleBase::cCapsuleBase( )
{

}
cCapsuleBase::~cCapsuleBase( )
{

}
bool cCapsuleBase::deleteThis()
{
	return false;
}
Game::Weapons::SubWeapon::SubWeaponType cCapsuleBase::getType()
{
	return mType;
}
void cCapsuleBase::setType(const Game::Weapons::SubWeapon::SubWeaponType type)
{
	mType = type;
}
ci::vec3 cCapsuleBase::getPos()
{
	return mPos;
}
ci::vec3 cCapsuleBase::getScale()
{
	return mScale;
}
void cCapsuleBase::setPos(const ci::vec3 pos)
{
	mPos = pos;
}
void cCapsuleBase::setScale(const ci::vec3 scale)
{
	mScale = scale;
}

ci::AxisAlignedBox cCapsuleBase::getAABB()
{
	return mAABB;
}

bool cCapsuleBase::getIsget()
{
	return mIsget;
}

void cCapsuleBase::setIsget(const bool isget)
{
	mIsget = isget;
}

}
}
