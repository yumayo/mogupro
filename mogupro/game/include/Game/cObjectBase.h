#pragma once
#include "cinder/app/App.h"
namespace Game {
	class cObjectBase {
	public:
		cObjectBase() {
			mPos = ci::vec2(0);
		}
		virtual void setup() = 0;
		virtual void update(const float& delta_time) = 0;
		virtual void draw() = 0;
		ci::vec2 getPos() {
			return mPos;
		}
		ci::vec2& getReferencePos() {
			return mPos;
		}
	protected:
		ci::vec2 mPos;
		bool mIsActive;
	};
}