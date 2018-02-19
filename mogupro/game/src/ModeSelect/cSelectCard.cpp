#include <ModeSelect/cSelectCard.h>
#include "Particle\cParticleManager.h"
#include"cinder\Easing.h"
#include "Resource\TextureManager.h"
#include"cinder\Rand.h"
namespace ModeSelect
{

cSelectCard::cSelectCard(const float angle, const std::string textureName, const ci::vec3 trancepos, const ci::vec3 sinlength,const int number)
{
	mAngle = angle;
	mBeginAngle = mAngle;
	mEndAngle = mAngle;
	mTextureName = textureName;
	mTrancePos = trancepos;
	mSinLength = sinlength;
	mNumber = number;
	//TEX->set(mTextureName, mTextureName);
	setPos();
	mScale = ci::vec3(1, 1, 0);
	mRandAngle = ci::randFloat(2.f*M_PI);
}
cSelectCard::~cSelectCard( )
{

}
void cSelectCard::draw()
{
	mDrawFunc.drawTextureRect3D(mPos, ci::vec3(mScale.x*mDrawRate.x,mScale.y*mDrawRate.y,0), ci::vec3(0, 0, M_PI), mTextureName, ci::ColorA(1,1,1,1));
}
void cSelectCard::update(const float & t)
{
	
	if (iseasing) {
		float speed = 3.f;
		e_t += speed*t;
		if (e_t >= 1.0f) {
			e_t = 1.0f;
			iseasing = false;
		}
		mAngle = EaseCubicOut(e_t, mBeginAngle, mEndAngle);
	}
	updateDrawRate(t);
	setPos();
	mRandAngle += t;
}
void cSelectCard::setEasing(const bool isleft)
{
	e_t = 0.0f;
	iseasing= true;
	if (mNumber == 0||mNumber==3) {
		mEndAngle = mNumber*M_PI / 2.f;
	}
	mBeginAngle = mEndAngle;
	if (isleft) {
		mNumber--;
		if (mNumber < 0) {
			mNumber = 3;
			mEndAngle = -M_PI / 2.f;
		}
		else {
			mEndAngle = mNumber*M_PI / 2.f;
		}
	
	}
	else {
		mNumber++;
		mNumber = mNumber % 4;
		if (mNumber == 0) {
			mEndAngle = 2.f*M_PI;
		}
		else {
			mEndAngle = mNumber*M_PI / 2.f;
		}
	}
	

	
	mAngle = mEndAngle;
}
const bool cSelectCard::getIsEasing()
{
	return iseasing;
}

void cSelectCard::setPos()
{
	if (mNumber == 0) {
		mPos = mDrawTrancePos+ mTrancePos + ci::vec3(mSinLength.x*sin(mAngle), mSinLength.y*cos(mAngle + M_PI), mSinLength.z*cos(mAngle + M_PI));
	}
	else {
		mPos =ci::vec3(0,(mSinLength.y/8.f)*sin(mRandAngle),0)+  mTrancePos + ci::vec3(mSinLength.x*sin(mAngle), mSinLength.y*cos(mAngle + M_PI), mSinLength.z*cos(mAngle + M_PI));
	}

}
void cSelectCard::updateDrawRate(const float & t)
{
	if (mNumber == 0) {//選択されていれば
		float speed = 3.f;
		if (isdown) {
			downT += speed*t;
			trancespeed -= 0.05f*t;//重力処理
			if (downT >= 1.0f) {
				downT = 1.0f;
				upT = 0.0f;
				isdown = false;
				trancespeed = 0.045f;//jump
			}
			mDrawRate.x = EaseCubicOut(downT, 1.0f, 1.10f);
			mDrawRate.y = EaseCubicOut(downT, 1.0f, 0.8f);
		}
		else {
			upT += speed*t;
			trancespeed -= 0.22*t;//重力処理
			if (upT >= 1.0f) {
				upT = 1.0f;
				downT = 0.0f;
				isdown = true;
				trancespeed = 0.0f;
				mDrawTrancePos = ci::vec3(0);
			}
			mDrawRate.x = EaseCubicOut(upT, 1.10f, 1.0f);
			mDrawRate.y = EaseCubicOut(upT, 0.8f, 1.0f);
		}
		mDrawTrancePos += ci::vec3(0, trancespeed, 0);
		if (mDrawTrancePos.y < -0.175014) {
			mDrawTrancePos.y = -0.175014;//デルタの暴走を止めるです
		}
	}
	else {
		isdown = true;
		mDrawRate = ci::vec2(1, 1);
		downT = 0.0f;
		upT = 0.f;
		mDrawTrancePos = ci::vec3(0);
		trancespeed = 0.0f;
	}
}
}
