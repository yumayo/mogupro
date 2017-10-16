#include <Game/Field/cBlock.h>
#include <cinder/gl/gl.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
static vec3 vertex0[] = {
    vec3( -0.5f,  0.5f, 0.5f ),
    vec3( 0.5f, -0.5f, 0.5f ),
    vec3( 0.5f,  0.5f, 0.5f ),
    vec3( -0.5f,  0.5f, 0.5f ),
    vec3( -0.5f, -0.5f, 0.5f ),
    vec3( 0.5f, -0.5f, 0.5f ), };
static vec3 vertex1[] = {
    vec3( -0.5f, -0.5f, -0.5f ),
    vec3( 0.5f,  0.5f, -0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ),
    vec3( -0.5f, -0.5f, -0.5f ),
    vec3( -0.5f,  0.5f, -0.5f ),
    vec3( 0.5f,  0.5f, -0.5f ), };
static vec3 vertex2[] = {
    vec3( -0.5f, -0.5f,  0.5f ),
    vec3( -0.5f,  0.5f, -0.5f ),
    vec3( -0.5f, -0.5f, -0.5f ),
    vec3( -0.5f, -0.5f,  0.5f ),
    vec3( -0.5f,  0.5f,  0.5f ),
    vec3( -0.5f,  0.5f, -0.5f ), };
static vec3 vertex3[] = {
    vec3( 0.5f,  0.5f,  0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ),
    vec3( 0.5f,  0.5f, -0.5f ),
    vec3( 0.5f,  0.5f,  0.5f ),
    vec3( 0.5f, -0.5f,  0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ), };
static vec3 vertex4[] = {
    vec3( -0.5f, 0.5f, -0.5f ),
    vec3( 0.5f, 0.5f,  0.5f ),
    vec3( 0.5f, 0.5f, -0.5f ),
    vec3( -0.5f, 0.5f, -0.5f ),
    vec3( -0.5f, 0.5f,  0.5f ),
    vec3( 0.5f, 0.5f,  0.5f ), };
static vec3 vertex5[] = {
    vec3( -0.5f, -0.5f,  0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ),
    vec3( 0.5f, -0.5f,  0.5f ),
    vec3( -0.5f, -0.5f,  0.5f ),
    vec3( -0.5f, -0.5f, -0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ), };

static vec3 normal0[] = {
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ), };
static vec3 normal1[] = {
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ), };
static vec3 normal2[] = {
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ), };
static vec3 normal3[] = {
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ), };
static vec3 normal4[] = {
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ), };
static vec3 normal5[] = {
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ), };


GLfloat cube_vtx[] = {
    // 前
    //  x,     y,    z,
    -0.5f,  0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,

    // 後
    -0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,

    // 右
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,

    // 左
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,

    // 上
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f,  0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f,  0.5f,
    0.5f, 0.5f,  0.5f,

    // 底
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
};

GLfloat cube_normal[] = {
    // 前
    // x,    y,    z,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    // 後
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    // 右
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    // 左
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    // 上
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    // 下
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
};

std::vector<vec2> getUv( int side_num )
{
    std::vector<vec2> uv = std::vector<vec2>( side_num * 4 );
    int c = 0;
    for (int i = 0; i < side_num; i++)
    {
        uv[c++] = vec2( 0.00000, 1.00000 );
        uv[c++] = vec2( 1.00000, 0.00000 );
        uv[c++] = vec2( 0.00000, 0.00000 );
        uv[c++] = vec2( 1.00000, 1.00000 );
    }
    return uv;
}

int getIndex( const vec3 & vtx, const vec3 & normal,
              const std::vector<vec3>& vertices,
              const std::vector<vec3>& normals )
{
    for (uint i = 0; i < (vertices.size()); ++i)
    {
        if (vertices[i] == vtx
             && normals[i] == normal)
        {
            // 全部一致
            return i;
        }
    }
    return -1;
}

cBlock::cBlock( const ci::vec3& position, const float& scale, const uint& num )
{
    mPosition = position;
    mScale = scale;
    mIsActive = true;
    mNum = num;
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
void cBlock::createSide( const  int& offset_index )
{
    if (mIsActive == false)
        return;
    if (mDrawSide.size() <= 0)
        return;

    mUv = getUv( mDrawSide.size() );

    // DrawElements用のデータを作成
    for (int i = 0; i < 36; ++i)
    {
        int side_num = i / 6;
        if (!std::any_of( mDrawSide.begin(), mDrawSide.end(),
                          [&]( int n ) { return side_num == n; } ))
            continue;

        vec3 vertex = vec3( cube_vtx[i * 3 + 0],
                            cube_vtx[i * 3 + 1],
                            cube_vtx[i * 3 + 2] ) + mPosition;
        vec3 normal = vec3( cube_normal[i * 3 + 0],
                            cube_normal[i * 3 + 1],
                            cube_normal[i * 3 + 2] );

        int id = getIndex( vertex, normal,
                           mVertices, mNormals );

        if (id < 0)
        {
            // 同じ座標と法線を持つ頂点は見つからず
            mIndices.emplace_back( mVertices.size() + offset_index );

            mVertices.emplace_back( vertex );
            mNormals.emplace_back( normal );
        }
        else
        {
            // 見つかった
            mIndices.emplace_back( id + offset_index );
        }
    }
}
void cBlock::clear()
{
    mVertices.clear();
    mIndices.clear();
    mUv.clear();
    mNormals.clear();
    mDrawSide.clear();
}
void cBlock::toBreak()
{
    mIsActive = false;
    clear();
}
}
}