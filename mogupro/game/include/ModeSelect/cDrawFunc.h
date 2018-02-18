#pragma once
#include <cinder/gl/gl.h>
namespace ModeSelect
{
class cDrawFunc
{
public:
    cDrawFunc( );
    ~cDrawFunc( );
	void drawTextureRect3D(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 rotate,const std::string texname, ci::ColorA color);
	void drawTextureRect2D(const ci::vec2 pos, const ci::vec2 scale, float rotate, const std::string texname, ci::ColorA color);
	void drawTextureCube(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 rotate, const std::string texname, ci::ColorA color);
private:
};
}
