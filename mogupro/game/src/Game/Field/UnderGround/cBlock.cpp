#include <Game/Field/UnderGround/cBlock.h>
#include <cinder/gl/gl.h>

using namespace ci;

namespace Game
{
namespace Field
{
namespace UnderGround
{
vec3 vertex0[] = {
    vec3( -0.50000, -0.50000,  0.50000 ),
    vec3( -0.50000,  0.50000,  0.50000 ),
    vec3( 0.50000,  0.50000,  0.50000 ),
    vec3( 0.50000, -0.50000,  0.50000 ), };
vec3 vertex1[] = {
    vec3( 0.50000, -0.50000, -0.50000 ),
    vec3( 0.50000,  0.50000, -0.50000 ),
    vec3( -0.50000,  0.50000, -0.50000 ),
    vec3( -0.50000, -0.50000, -0.50000 ), };
vec3 vertex2[] = {
    vec3( -0.50000, -0.50000, -0.50000 ),
    vec3( -0.50000,  0.50000, -0.50000 ),
    vec3( -0.50000,  0.50000,  0.50000 ),
    vec3( -0.50000, -0.50000,  0.50000 ), };
vec3 vertex3[] = {
    vec3( 0.50000, -0.50000,  0.50000 ),
    vec3( 0.50000,  0.50000,  0.50000 ),
    vec3( 0.50000,  0.50000, -0.50000 ),
    vec3( 0.50000, -0.50000, -0.50000 ), };
vec3 vertex4[] = {
    vec3( -0.50000,  0.50000,  0.50000 ),
    vec3( -0.50000,  0.50000, -0.50000 ),
    vec3( 0.50000,  0.50000, -0.50000 ),
    vec3( 0.50000,  0.50000,  0.50000 ), };
vec3 vertex5[] = {
    vec3( -0.50000, -0.50000, -0.50000 ),
    vec3( -0.50000, -0.50000,  0.50000 ),
    vec3( 0.50000, -0.50000,  0.50000 ),
    vec3( 0.50000, -0.50000, -0.50000 ), };


std::vector<uint32_t> getIndices( int side_size )
{
    std::vector<GLuint> indices = std::vector<GLuint>( side_size * 6 );
    int c = 0;
    for (int i = 0; i < side_size * 4; i += 4)
    {
        indices[c++] = i;
        indices[c++] = i + 2;
        indices[c++] = i + 1;
        indices[c++] = i + 2;
        indices[c++] = i;
        indices[c++] = i + 3;
    }
    return indices;
}

std::vector<vec2> getUv( int side_num )
{
    std::vector<vec2> uv = std::vector<vec2>( side_num * 4 );
    int c = 0;
    for (int i = 0; i < side_num; i++)
    {
        uv[c++] = vec2( 0.00000, 1.00000 );
        uv[c++] = vec2( 0.00000, 0.00000 );
        uv[c++] = vec2( 1.00000, 0.00000 );
        uv[c++] = vec2( 1.00000, 1.00000 );
    }
    return uv;
}

vec3* getVertices( int side_num )
{
    switch (side_num)
    {
        case 0: return vertex0;
        case 1: return vertex1;
        case 2: return vertex2;
        case 3: return vertex3;
        case 4: return vertex4;
        case 5: return vertex5;
    }
}

cBlock::cBlock( const ci::vec3 position, const float scale )
{
    mPosition = position;
    mScale = scale;
    mIsActive = true;
}
cBlock::~cBlock()
{
}
void cBlock::update()
{
}
void cBlock::draw()
{
    if (mIsActive == false) return;
    gl::pushModelView();
    gl::translate( mPosition );
    gl::scale( vec3( mScale ) );
    gl::drawCube( vec3( 0 ), vec3( 1 ) );
    gl::popModelView();
}
void cBlock::drawMesh()
{
    if (mIsActive == false) return;
    gl::pushModelView();
    gl::translate( mPosition );
    gl::scale( vec3( mScale ) );
    //gl::draw( mVboMesh );
    gl::draw( mMesh );
    gl::popModelView();
}
void cBlock::setupDrawSide( std::vector<int>* draw_side )
{
    for (size_t i = 0; i < draw_side->size(); i++)
    {
        vec3* temp = getVertices( (*draw_side)[i] );
        for (int v = 0; v < 4; v++)
            mVertices.emplace_back( temp[v] );
    }
    mIndices = getIndices( draw_side->size() );
    mUv = getUv( draw_side->size() );

    // ÉÅÉbÉVÉÖÇçÏÇÈ
    if (mVertices.size() > 0)
        for (size_t i = 0; i < mVertices.size(); i++)
            mMesh.appendPositions( &mVertices[0], mVertices.size() );
    if (mIndices.size() > 0)
        mMesh.appendIndices( &mIndices[0], mIndices.size() );
    if (mUv.size() > 0)
        mMesh.appendTexCoords0( &mUv[0], mUv.size() );

    // VBOÇÃé¿ëïÇÕÇ‡Ç§ÇøÇÂÇ¢êÊ
    //mVboMesh = gl::VboMesh::create( mMesh );
}
void cBlock::toBreak()
{
    mIsActive = false;
}
}
}
}
