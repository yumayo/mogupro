#define FONTSTASH_IMPLEMENTATION
#include <Node/Library/font.h>
#include <cinder/TriMesh.h>
#include <cinder/gl/gl.h>
namespace Node
{
namespace Library
{
int font::create( void * userPtr, int width, int height ) noexcept
{
    Context* gl = (Context*)userPtr;

    // TIPS:テクスチャ内部形式をGL_R8にしといて
    //      シェーダーでなんとかする方式(from nanoVG)
    gl->atlas = cinder::gl::Texture2d::create( width, height,
                                               cinder::gl::Texture2d::Format( ).dataType( GL_UNSIGNED_BYTE ).internalFormat( GL_RED ) );

    gl->width = width;
    gl->height = height;

    return 1;
}
int font::resize( void * userPtr, int width, int height ) noexcept
{
    // Reuse create to resize too.
    return create( userPtr, width, height );
}
void font::update( void * userPtr, int * rect, const unsigned char * data ) noexcept
{
    Context* gl = (Context*)userPtr;
    if ( !gl->atlas.get( ) ) return;

    int w = rect[2] - rect[0];
    int h = rect[3] - rect[1];

    // TIPS:data側も切り抜いて転送するので
    //      その指定も忘れない
    glPixelStorei( GL_UNPACK_ROW_LENGTH, gl->width );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, rect[0] );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, rect[1] );

    gl->atlas->update( data, GL_RED, GL_UNSIGNED_BYTE, 0, w, h, ci::ivec2( rect[0], rect[1] ) );

    // 設定した後はもとに戻さないとその後のテクスチャ達が壊れてしまう。
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
}
// FIXME: ci::glのコードを参考にした
//        ちょいと重い
void font::draw( void * userPtr, const float * verts, const float * tcoords, const unsigned int * colors, int nverts ) noexcept
{
    using namespace ci;

    Context* gl = (Context*)userPtr;
    if ( !gl->atlas.get( ) )
        return;

    auto* ctx = cinder::gl::context( );
    const cinder::gl::GlslProg* curGlslProg = ctx->getGlslProg( );

    size_t totalArrayBufferSize = sizeof( float ) * nverts * 2
        + sizeof( float ) * nverts * 2
        + nverts * 4;

    ctx->pushVao( );
    ctx->getDefaultVao( )->replacementBindBegin( );

    cinder::gl::VboRef defaultArrayVbo = ctx->getDefaultArrayVbo( totalArrayBufferSize );

    cinder::gl::ScopedBuffer vboScp( defaultArrayVbo );
    cinder::gl::ScopedTextureBind texScp( gl->atlas );

    size_t curBufferOffset = 0;
    {
        int loc = curGlslProg->getAttribSemanticLocation( geom::Attrib::POSITION );
        cinder::gl::enableVertexAttribArray( loc );
        cinder::gl::vertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)curBufferOffset );
        defaultArrayVbo->bufferSubData( curBufferOffset, sizeof( float ) * nverts * 2, verts );
        curBufferOffset += sizeof( float ) * nverts * 2;
    }

    {
        int loc = curGlslProg->getAttribSemanticLocation( geom::Attrib::TEX_COORD_0 );
        cinder::gl::enableVertexAttribArray( loc );
        cinder::gl::vertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)curBufferOffset );
        defaultArrayVbo->bufferSubData( curBufferOffset, sizeof( float ) * nverts * 2, tcoords );
        curBufferOffset += sizeof( float )* nverts * 2;
    }

    {
        int loc = curGlslProg->getAttribSemanticLocation( geom::Attrib::COLOR );
        cinder::gl::enableVertexAttribArray( loc );
        cinder::gl::vertexAttribPointer( loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)curBufferOffset );
        defaultArrayVbo->bufferSubData( curBufferOffset, nverts * 4, colors );
        // curBufferOffset += nverts * 4;
    }

    ctx->getDefaultVao( )->replacementBindEnd( );

    ctx->setDefaultShaderVars( );
    ctx->drawArrays( GL_TRIANGLES, 0, nverts );
    ctx->popVao( );
}
font::font( const int width, const int height, const int flags ) noexcept
{
    FONSparams params;

    memset( &params, 0, sizeof( params ) );
    params.width = width;
    params.height = height;
    params.flags = (unsigned char)flags;

    params.renderCreate = font::create;
    params.renderResize = font::resize;
    params.renderUpdate = font::update;
    params.renderDraw = font::draw;
    params.renderDelete = nullptr;

    params.userPtr = &gl_;

    context_ = fonsCreateInternal( &params );
}
font::~font( ) noexcept
{
    fonsDeleteInternal( context_ );
}
font::operator FONScontext*( ) noexcept
{
    return context_;
}
font::operator cinder::gl::Texture2dRef( ) noexcept
{
    return gl_.atlas;
}
unsigned int font::color( const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a ) noexcept
{
    return ( r ) | ( g << 8 ) | ( b << 16 ) | ( a << 24 );
}
unsigned int font::color( cinder::ColorA value ) noexcept
{
    return color( value.r, value.g, value.b, value.a );
}
unsigned int font::color( const float r, const float g, const float b, const float a ) noexcept
{
    unsigned char r8 = (unsigned char)(r * 255.0f);
    unsigned char g8 = (unsigned char)(g * 255.0f);
    unsigned char b8 = (unsigned char)(b * 255.0f);
    unsigned char a8 = (unsigned char)(a * 255.0f);

    return color( r8, g8, b8, a8 );
}
}
}