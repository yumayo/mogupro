#include <Game/Light/cSpotLightParam.h>
#include <Game/cLightManager.h>
namespace Game
{
namespace Light
{
cSpotLightParam::~cSpotLightParam( )
{
	cLightManager::getInstance( )->removeSpotLight( id, this );
}
void cSpotLightParam::reAttachPosition( cinder::vec3 position )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->position = position;
	cLightManager::getInstance( )->attachChunk( this );
}
void cSpotLightParam::reAttachDirection( cinder::vec3 direction )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->direction = direction;
	cLightManager::getInstance( )->attachChunk( this );
}
void cSpotLightParam::reAttachRadius( float radius )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( this );
}
void cSpotLightParam::reAttachPositionWithDirection( cinder::vec3 position, cinder::vec3 direction )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->position = position;
	this->direction = direction;
	cLightManager::getInstance( )->attachChunk( this );
}
void cSpotLightParam::reAttachPositionWithRadius( cinder::vec3 position, float radius )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->position = position;
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( this );
}
void cSpotLightParam::reAttachDirectionWithRadius( cinder::vec3 direction, float radius )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->direction = direction;
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( this );
}
void cSpotLightParam::reAttachPositionWithDirectionWithRadius( cinder::vec3 position, cinder::vec3 direction, float radius )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->position = position;
	this->direction = direction;
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( this );
}
}
}
