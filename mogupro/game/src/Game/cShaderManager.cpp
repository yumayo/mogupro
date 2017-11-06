#include <Game/cShaderManager.h>
#include <cinder/gl/gl.h>
#include <Game/cGemManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cPlayerManager.h>
using namespace ci;
namespace Game
{
void cShaderManager::setup( )
{
	mGlsl = gl::GlslProg::create( app::loadAsset( "Shader/world.vert" ),
								  app::loadAsset( "Shader/world.frag" ) );
}
void cShaderManager::draw( std::function<void( )> render )
{
	gl::ScopedGlslProg scpGlsl( mGlsl );
	mGlsl->uniform( "uAmb", ColorA( 99 / 255.0F, 161 / 255.0F, 255 / 255.0F, 1.0F ) );
	std::vector<vec4> lightPositions;
	std::vector<vec4> lightColors;
	int lightNum = glm::min( (int)GemManager->getGems( ).size( ), 100 );
	mGlsl->uniform( "uLightNum", lightNum );
	for ( int i = 0; i < lightNum - Game::cPlayerManager::getInstance()->getPlayers().size(); ++i )
	{
		lightPositions.emplace_back( CAMERA->getCamera( ).getViewMatrix( ) *
									 vec4( vec3( GemManager->getGems( )[i]->getPos( ) ), 1 ) );
		lightColors.emplace_back( vec4( vec3( GemManager->getGems( )[i]->getColor( ) ), 1 ) );
	}
    auto const& players = Game::cPlayerManager::getInstance()->getPlayers();

    for ( int i = 0; i < players.size(); ++i )
    {
        lightPositions.emplace_back( CAMERA->getCamera().getViewMatrix() * vec4( players[i]->getPos(), 1 ) );
        switch ( players[i]->getWhichTeam() )
        {
            case Game::Player::Team::Blue:
                lightColors.emplace_back( vec4( 0.8, 0.8, 1, 1 ) );
                break;
            case Game::Player::Team::Red:
                lightColors.emplace_back( vec4( 1, 0.8, 0.8, 1 ) );
                break;
            default:
                break;
        }
    }
    
	mGlsl->uniform( "uModelViewLightPositions", lightPositions.data( ), lightNum );
	mGlsl->uniform( "uModelViewLightColors", lightColors.data( ), lightNum );

	render( );
}
}
