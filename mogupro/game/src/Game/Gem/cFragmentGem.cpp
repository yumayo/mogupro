#include <Game/Gem/cFragmentGem.h>
#include <Log/Log.h>
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
			Log::cLogManager::getInstance()->add("remove");
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
			mRb.setSpeed(vec3(0,10,0));
			mRb.setFriction(1.0f);
			mIsRigid = true;
			mVisible = true;
		}


		void cFragmentGem::draw()
		{
			if (!mVisible) return;
			ci::gl::color(ci::Color(mColorA));
			drawCube(vec3(mPosition), vec3(mScale));
			color(ColorA(1,1,1,1));
		}


		void cFragmentGem::lateUpdate(const float& delta_time)
		{
			if (!mIsRigid) return;
			mPosition = mAabb.getPosition();
			mSpeed = mRb.getSpeed();
		}


		void cFragmentGem::update()
		{

		}


		void cFragmentGem::setIsRigid(bool isRigid)
		{
			if (isRigid == mIsRigid) return;

			if (isRigid)
			{
				mRb.addWorld();
				mIsRigid = true;
			}
			else
			{
				mRb.removeWorld();
				
				Log::cLogManager::getInstance()->writeLog("remove", "remove rigidbody");
				mIsRigid = false;
			}
		}


		void cFragmentGem::setVisible(bool visible)
		{
			mVisible = visible;
		}
	}
}
