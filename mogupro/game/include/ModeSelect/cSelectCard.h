#pragma once
#include <cinder/gl/gl.h>
#include <ModeSelect\cDrawFunc.h>
namespace ModeSelect
{
class cSelectCard
{
public:
    cSelectCard(const float angle,const std::string textureName,const ci::vec3 trancepos,const ci::vec3 sinlength,const int number);
    ~cSelectCard( );
	void draw();
	void update(const float& t);
	void setEasing(const bool isleft);
	const bool getIsEasing();
private:
	ci::vec3 mPos;
	ci::vec3 mScale;
	std::string mTextureName;
	ModeSelect::cDrawFunc mDrawFunc;
	float mAngle;
	float mBeginAngle;
	float mEndAngle;
	bool iseasing = false;
	ci::vec3 mTrancePos;
	ci::vec3 mSinLength;
	ci::vec2 mDrawRate = ci::vec2(1, 1);
	int mNumber;
	float e_t = 0.0f;
	void setPos();
	void updateDrawRate(const float & t);
	float upT = 0.0f;
	float downT = 0.0f;
	bool isdown = false;
	ci::vec3 mDrawTrancePos = ci::vec3(0,0,0);
	float trancespeed = 0.0f;
	float mRandAngle = 0.0f;
};
}
