#include <Game/SkyDome/cSkyDome.h>
#include <Resource/cObjectManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/Field/FieldData.h>
void Game::SkyDome::cSkyDome::setup()
{
	worldCenter = ci::vec3( Game::Field::CHUNK_RANGE_X, Game::Field::CHUNK_RANGE_Y, Game::Field::CHUNK_RANGE_Z ) * Game::Field::BLOCK_SIZE * (float)Game::Field::CHUNK_SIZE / 2.0F;
	mGlsl = ci::gl::getStockShader( ci::gl::ShaderDef( ).color( ) );
	caveMesh = Resource::cObjectManager::getInstance( )->findObject( "cave.obj" );
}

void Game::SkyDome::cSkyDome::draw()
{
	ci::gl::ScopedColor col( ci::ColorA( 0.294, 0.156, 0.09 ) );
	ci::gl::ScopedModelMatrix mat;
	ci::gl::translate( worldCenter );
	ci::gl::draw( caveMesh );
}
