#include "cinder/gl/Context.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "stb_image.h"

class Texture
{
public:
	Texture()
	{

	}
	~Texture()
	{
		glDeleteTextures(1, &textureId);
	}
	Texture(const char* path_)
	{

	}

	unsigned int textureId;

	int w, h;
	int comp; //3 or 4  (RGP or RGPA) 
	unsigned char* image;

	void imageLoad(const char* path_)
	{
		image = stbi_load(path_, &w, &h, &comp, 0);
	}

	void imageBind()
	{
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);

		GLuint type = (comp == 3) ? GL_RGB : GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D,
			0,                     //mipmapレベル
			type, w, h,            //内部形式とサイズ
			0, type,               //境界色、形式
			GL_UNSIGNED_BYTE,
			image);

		stbi_image_free(image);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

//void drawQuad1(const ci::Vec2f& pos_, const ci::Vec2f& size_, const ci::ColorA& colorA_)
//{
//	const GLfloat vtx[] =
//	{
//		pos_.x - size_.x / 2, pos_.y + size_.y / 2,
//		pos_.x + size_.x / 2, pos_.y + size_.y / 2,
//		pos_.x + size_.x / 2, pos_.y - size_.y / 2,
//
//		pos_.x - size_.x / 2, pos_.y + size_.y / 2,
//		pos_.x + size_.x / 2, pos_.y - size_.y / 2,
//		pos_.x - size_.x / 2, pos_.y - size_.y / 2
//	};
//
//	glVertexPointer(2, GL_FLOAT, 0, vtx);
//
//	glColor4f(colorA_.r, colorA_.g, colorA_.b, colorA_.a);
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//
//	// 三角形を描画
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//}
//
//void drawTexture(const ci::Vec2f& pos_, const ci::Vec2f& size_, const Texture1& texture_, const ci::ColorA& colorA_)
//{
//	glBindTexture(GL_TEXTURE_2D, texture_.textureId);
//	//各隊縮小時のフィルタ
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//GL_NEAREST
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	GLfloat uv[] =
//	{
//		0.0,0.0,
//		1.0,0.0,
//		1.0,1.0,
//
//		0.0,0.0,
//		1.0,1.0,
//		0.0,1.0
//	};
//
//	glTexCoordPointer(2, GL_FLOAT, 0, uv);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glEnable(GL_TEXTURE_2D);
//
//	drawQuad1(pos_, size_, colorA_);
//
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	glDisable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}