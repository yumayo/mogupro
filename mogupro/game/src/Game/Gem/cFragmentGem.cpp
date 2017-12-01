#include <Game/Gem/cFragmentGem.h>
namespace Game
{
	namespace Gem
	{
		cFragmentGem::cFragmentGem(int id, vec3 position, vec3 scale, ColorA color, GemType type) :
			mId(id), mPosition(position), mScale(scale), mColorA(color), mType(type), mAabb(position, scale), mRb(mAabb)
		{
			mIsActive    = true;
			mSinRotate   = 0.0f;
			mPutPos      = vec3(0.0f);
			mSpeed       = vec3(0, 5, 0);
		};


		cFragmentGem::~cFragmentGem()
		{
			mAabb.removeWorld();
			mRb.removeWorld();
		}


		void cFragmentGem::setup()
		{
			mAabb.addWorld();
			mRb.addWorld();
			mRb.setSpeed(vec3(0,5,0));
			mRb.setFriction(1.0f);
		}


		void cFragmentGem::draw()
		{
			ci::gl::color(ci::Color(mColorA));
			drawCube(vec3(mPosition), vec3(mScale));
			color(ColorA(1,1,1,1));
		}


		void cFragmentGem::lateUpdate(const float& delta_time)
		{
			mPosition = mAabb.getPosition();
			mSpeed = mRb.getSpeed();
		}


		void cFragmentGem::update()
		{

		}
	}
}
