#include <Game/Light/cLineLightParam.h>
#include <Game/cLightManager.h>
namespace Game
{
namespace Light
{
cLineLightParam::~cLineLightParam( )
{
	cLightManager::getInstance( )->removeLineLight( id, this );
}
void cLineLightParam::reAttachLine( cinder::vec3 beginPosition, cinder::vec3 endPosition )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->beginPosition = beginPosition;
	this->endPosition = endPosition;
	cLightManager::getInstance( )->attachChunk( this );
}
void cLineLightParam::reAttachRadius( float radius )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( this );
}
void cLineLightParam::reAttachLineWithRadius( cinder::vec3 beginPosition, cinder::vec3 endPosition, float radius )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->beginPosition = beginPosition;
	this->endPosition = endPosition;
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( this );
}
}
}
