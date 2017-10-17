#include <Game/cFieldManager.h>
#include <cinder/gl/gl.h>
using namespace ci;
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
void cFieldManager::update( const float& delta_time )
{
    mUnderGround.update();
}
void cFieldManager::draw()
{
    auto ctx = gl::context();
    bool is_culling_change = ctx->getBoolState( GL_CULL_FACE );

    if (is_culling_change == false)
        gl::enableFaceCulling( true );

    auto lambert = gl::ShaderDef().lambert();
    auto shader = gl::getStockShader( lambert );
    gl::ScopedGlslProg shaderScp( shader );

    mUnderGround.draw();

    if (is_culling_change == false)
        gl::enableFaceCulling( false );
}
bool cFieldManager::blockBreak( const ci::vec3& position, const float& radius )
{
    return mUnderGround.blockBreak( position, radius );
}
bool cFieldManager::isOutOfRange( const ci::ivec3 & cell_num )
{
    return mUnderGround.isOutOfRange( cell_num );
}
ci::vec3 cFieldManager::getBlockCenterTopPosition( const ci::vec3 & target_position )
{
    return mUnderGround.getBlockCenterTopPosition( target_position );
}
}