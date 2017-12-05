#include <Game/Light/cLineLightParam.h>
#include <Game/cLightManager.h>
namespace Game
{
namespace Light
{
cLineLightParam::~cLineLightParam( )
{

}
void cLineLightParam::reAttachLine( Utility::softptr<cLineLightParam> handle, cinder::vec3 beginPosition, cinder::vec3 endPosition )
{
	cLightManager::getInstance( )->detachChunk( handle );
	this->beginPosition = beginPosition;
	this->endPosition = endPosition;
	cLightManager::getInstance( )->attachChunk( handle );
}
void cLineLightParam::reAttachRadius( Utility::softptr<cLineLightParam> handle, float radius )
{
	cLightManager::getInstance( )->detachChunk( handle );
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( handle );
}
void cLineLightParam::reAttachLineWithRadius( Utility::softptr<cLineLightParam> handle, cinder::vec3 beginPosition, cinder::vec3 endPosition, float radius )
{
	cLightManager::getInstance( )->detachChunk( handle );
	this->beginPosition = beginPosition;
	this->endPosition = endPosition;
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( handle );
}
}
}
