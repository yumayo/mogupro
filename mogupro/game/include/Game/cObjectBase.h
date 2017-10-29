#pragma once
#include "cinder/app/App.h"
namespace Game {
	class cObjectBase {
	public:
		cObjectBase()
        : mPos(cinder::vec3(0.0F)), mIsActive(false) {}
		cObjectBase(const ci::vec3& pos)
        {
			mPos = pos;
            mIsActive = false;
		}
		virtual void setup() = 0;
		virtual void update(const float& delta_time) = 0;
		virtual void draw() = 0;
		ci::vec3 getPos() {
			return mPos;
		}
		ci::vec3& getReferencePos() {
			return mPos;
		}
		void setPos(const ci::vec3 pos) {
			mPos = pos;
		}
		bool isActive() {
			return mIsActive;
		}
	protected:
		ci::vec3 mPos;
		bool mIsActive;
	};
}