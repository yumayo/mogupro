#include <Game/cFieldManager.h>
#include <cinder/gl/gl.h>
namespace Game
{
cFieldManager::cFieldManager()
{

}
cFieldManager::~cFieldManager()
{
}
void cFieldManager::setup()
{
    mUnderGround.setup();
}
void cFieldManager::update( const float delta_time )
{
    mUnderGround.update();
}
void cFieldManager::draw()
{
    ci::gl::enableFaceCulling( true );
    auto lambert = ci::gl::ShaderDef().lambert();
    auto shader = ci::gl::getStockShader( lambert );
    ci::gl::ScopedGlslProg shaderScp( shader );
    mUnderGround.draw();
    ci::gl::enableFaceCulling( false );
}
void cFieldManager::blockBreak( ci::vec3 position, float  radius )
{
    mUnderGround.blockBreak( position, radius );
}
}