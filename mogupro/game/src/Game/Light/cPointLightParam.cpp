#include <Game/Light/cPointLightParam.h>
#include <cinder/gl/gl.h>
#include <Game/cLightManager.h>
namespace Game
{
namespace Light
{
void cPointLightParam::reAttachPosition( Utility::softptr<cPointLightParam> handle, cinder::vec3 position )
{
	cLightManager::getInstance( )->detachChunk( handle );
	this->position = position;
	cLightManager::getInstance( )->attachChunk( handle );
}
void cPointLightParam::reAttachRadius( Utility::softptr<cPointLightParam> handle, float radius )
{
	cLightManager::getInstance( )->detachChunk( handle );
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( handle );
}
void cPointLightParam::reAttachPositionWithRadius( Utility::softptr<cPointLightParam> handle, cinder::vec3 position, float radius )
{
	cLightManager::getInstance( )->detachChunk( handle );
	this->position = position;
	this->radius = radius;
	cLightManager::getInstance( )->attachChunk( handle );
}
}
}
