#include <Utility/cFont.h>
namespace Utility
{
void cFont::setup( std::string const & underAsset, float size )
{
	label.init( underAsset, size );
	label.set_scale( cinder::vec2( 1, -1 ) );
}
void cFont::setSize( float size )
{
	label.set_size( size );
}
void cFont::draw( cinder::vec2 position, std::string const & utf8Str )
{
	label.set_position( position );
	label.set_text( utf8Str );
	label.entry_render( cinder::mat4( ) );
}
}
