#include <ModeSelect/cDrawFunc.h>
#include "Resource\TextureManager.h"
#include"Resource\cImageManager.h"
using namespace ci;
using namespace ci::app;
namespace ModeSelect
{
cDrawFunc::cDrawFunc( )
{

}
cDrawFunc::~cDrawFunc( )
{

}
void cDrawFunc::drawTextureRect3D(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 rotate, const std::string texname, ci::ColorA color)
{
	cinder::gl::ScopedGlslProg p(cinder::gl::getStockShader(gl::ShaderDef().color().texture()));
	cinder::gl::ScopedTextureBind a(Resource::IMAGE[texname]);
	ci::gl::pushModelView();
	ci::gl::translate(pos);
	gl::rotate(rotate.x, ci::vec3(1, 0, 0));
	gl::rotate(rotate.y, ci::vec3(0, 1, 0));
	gl::rotate(rotate.z, ci::vec3(0, 0, 1));
	ci::gl::scale(scale);
	ci::gl::color(color);
	ci::gl::drawSolidRect(ci::Rectf(ci::vec2(-0.5, -0.5), ci::vec2(0.5, 0.5)));
	ci::gl::popModelView();
}
void cDrawFunc::drawTextureRect2D(const ci::vec2 pos, const ci::vec2 scale, float rotate, const std::string texname, ci::ColorA color)
{
	cinder::gl::ScopedGlslProg p(cinder::gl::getStockShader(gl::ShaderDef().color().texture()));
	cinder::gl::ScopedTextureBind a(Resource::IMAGE[texname]);
	ci::gl::pushModelView();
	ci::gl::translate(pos);
	gl::rotate(rotate, ci::vec3(0, 0, 1));
	ci::gl::scale(scale);
	ci::gl::color(color);
	ci::gl::drawSolidRect(ci::Rectf(ci::vec2(-0.5, -0.5), ci::vec2(0.5, 0.5)));
	ci::gl::popModelView();
}

void cDrawFunc::drawTextureCube(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 rotate, const std::string texname, ci::ColorA color)
{
	cinder::gl::ScopedGlslProg p(cinder::gl::getStockShader(gl::ShaderDef().color().texture()));
	cinder::gl::ScopedTextureBind a(Resource::IMAGE[texname]);
	ci::gl::pushModelView();
	ci::gl::translate(pos);
	gl::rotate(rotate.x, ci::vec3(1, 0, 0));
	gl::rotate(rotate.y, ci::vec3(0, 1, 0));
	gl::rotate(rotate.z, ci::vec3(0, 0, 1));
	ci::gl::scale(scale);
	ci::gl::color(color);
	ci::gl::drawCube(ci::vec3(0), ci::vec3(1));
	ci::gl::popModelView();
}


}
