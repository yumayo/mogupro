#include <Node/Renderer/surface.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
CREATE_CPP( surface, cinder::vec2 size, cinder::ColorA color )
{
    CREATE( surface, size, color );
}
bool surface::init( cinder::vec2 size, cinder::ColorA color )
{
    _surface = Surface8u( (int32_t)size.x, (int32_t)size.y, true );
    for ( int y = 0; y < _surface.getHeight( ); ++y )
    {
        for ( int x = 0; x < _surface.getWidth( ); ++x )
        {
            _surface.setPixel( vec2( x, y ), color );
        }
    }
    _texture = gl::Texture::create( _surface, createSpriteFormat( ) );
    _content_size = _surface.getSize( );
    return true;
}
void surface::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
void surface::set_pixel( cinder::vec2 pixel, cinder::ColorA color )
{
    _surface.setPixel( pixel, color );
    texture_update( pixel, ivec2( pixel ) + 1 );
}
void surface::paint_fill_circle( cinder::vec2 pixel, float radius, cinder::ColorA color )
{
    if ( radius < 0 ) return;

    Rectf rect( glm::floor( pixel - radius - 1.0F ), glm::ceil( pixel + radius ) );

    for ( int y = (int)rect.y1; y <= (int)rect.y2; ++y )
    {
        for ( int x = (int)rect.x1; x <= (int)rect.x2; ++x )
        {
            if ( radius < length( vec2( x, y ) - rect.getCenter( ) ) ) continue;
            if ( x < 0 || y < 0 || x >= _surface.getWidth( ) || y >= _surface.getHeight( ) ) continue;
            _surface.setPixel( ivec2( x, y ), ColorA8u( color ) );
        }
    }

    ivec2 min( ivec2( rect.x1, rect.y1 ) );
    min = glm::clamp( min, ivec2( 0 ), _surface.getSize( ) );
    ivec2 max( ivec2( rect.x2, rect.y2 ) );
    max = glm::clamp( max, ivec2( 0 ), _surface.getSize( ) );

    texture_update( min, max );
}
void surface::paint_fill_rect( cinder::Rectf rect, cinder::ColorA color )
{
    if ( rect.x2 < rect.x1 ) std::swap( rect.x1, rect.x2 );
    if ( rect.y2 < rect.y1 ) std::swap( rect.y1, rect.y2 );

    rect.x1 = glm::floor( rect.x1 );
    rect.y1 = glm::floor( rect.y1 );
    rect.x2 = glm::ceil( rect.x2 );
    rect.y2 = glm::ceil( rect.y2 );

    for ( int y = (int)rect.y1; y <= (int)rect.y2; ++y )
    {
        for ( int x = (int)rect.x1; x <= (int)rect.x2; ++x )
        {
            _surface.setPixel( ivec2( x, y ), ColorA8u( color ) );
        }
    }

    ivec2 min( ivec2( rect.x1, rect.y1 ) );
    min = glm::clamp( min, ivec2( 0 ), _surface.getSize( ) );
    ivec2 max( ivec2( rect.x2, rect.y2 ) );
    max = glm::clamp( max, ivec2( 0 ), _surface.getSize( ) );

    texture_update( min, max );
}
cinder::ColorA surface::get_pixel( cinder::vec2 pixel )
{
    return _surface.getPixel( pixel );
}
cinder::gl::Texture::Format surface::createSpriteFormat( )
{
    gl::Texture::Format format;
    return format;
}
void surface::texture_update( cinder::ivec2 min, cinder::ivec2 max )
{
    auto width = max.x - min.x;
    auto height = max.y - min.y;

    std::unique_ptr<unsigned char [ ]> data( new unsigned char[sizeof( unsigned char ) * 4 * width * height] );

    for ( int y = min.y; y < max.y; ++y )
    {
        for ( int x = min.x; x < max.x; ++x )
        {
            auto col = _surface.getPixel( ivec2( x, y ) );
            ivec2 pos = ivec2( x, y ) - min;
            data[( height - 1 - pos.y ) * 4 * width + pos.x * 4 + 0] = col.r;
            data[( height - 1 - pos.y ) * 4 * width + pos.x * 4 + 1] = col.g;
            data[( height - 1 - pos.y ) * 4 * width + pos.x * 4 + 2] = col.b;
            data[( height - 1 - pos.y ) * 4 * width + pos.x * 4 + 3] = col.a;
        }
    }

    glPixelStorei( GL_UNPACK_ROW_LENGTH, width );
    _texture->update( data.get( ), GL_RGBA, GL_UNSIGNED_BYTE, 0, width, height, ivec2( min.x, _surface.getHeight( ) - min.y - height ) );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
}
}
}