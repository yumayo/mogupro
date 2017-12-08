#include <Game/Light/cPointLightParam.h>
#include <Game/cLightManager.h>
namespace Game
{
namespace Light
{
cPointLightParam::~cPointLightParam( )
{
	cLightManager::getInstance( )->removePointLight( id, this );
}
void cPointLightParam::reAttachPosition( cinder::vec3 position )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->position = position;
	cLightManager::getInstance( )->attachChunk( this );
}
void cPointLightParam::reAttachRadius( float radius )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( this );
}
void cPointLightParam::reAttachPositionWithRadius( cinder::vec3 position, float radius )
{
	cLightManager::getInstance( )->detachChunk( this );
	this->position = position;
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( this );
}
}
}
