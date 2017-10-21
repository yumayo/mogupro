#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/gl/Texture.h>
#include <cinder/Camera.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/GlslProg.h>
namespace Shader
{
class cShadowManager : public Utility::cSingletonAble<cShadowManager>
{
public:
    void setup( );
    void update( std::function<void( )> const& drawFunc );
    void draw( std::function<void( )> const& drawFunc );
private:
    cinder::gl::Texture2dRef mShadowTex;
    cinder::CameraOrtho mCamera;
    cinder::gl::FboRef fbo;
    cinder::gl::GlslProgRef shader;
};
}
