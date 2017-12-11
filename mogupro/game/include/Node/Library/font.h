#pragma once
//
// フォントを扱う
// SOURCE: fontstash
//
#include <boost/noncopyable.hpp>
#include <cinder/gl/Texture.h>
#include <Node/library/fontstash.h>
namespace Node
{
namespace Library
{
class font : private boost::noncopyable
{
    struct Context
    {
        cinder::gl::Texture2dRef atlas;
        int width, height;
    };
    Context gl_;
    FONScontext* context_;
    // 以下、fontstashからのコールバック関数
    static int create( void* userPtr, int width, int height ) noexcept;
    static int resize( void* userPtr, int width, int height ) noexcept;
    static void update( void* userPtr, int* rect, const unsigned char* data ) noexcept;
    // FIXME: ci::glのコードを参考にした
    //        ちょいと重い
    static void draw( void* userPtr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts ) noexcept;
public:
    font( const int width, const int height, const int flags ) noexcept;
    ~font( ) noexcept;
    operator FONScontext* ( ) noexcept;
    operator cinder::gl::Texture2dRef( ) noexcept;
    static unsigned int color( const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a ) noexcept;
    static unsigned int color( cinder::ColorA value ) noexcept;
    static unsigned int color( const float r, const float g, const float b, const float a = 1.0f ) noexcept;
};
}
}