#pragma once
#include "cinder/app/App.h"

class ObjectBase {
public:
	ObjectBase() {
		mPos = ci::vec2(0);
	}
	virtual void setup() = 0;
	virtual void update(const float& delta_time) = 0;
	virtual void draw() = 0;

protected:
	ci::vec2 mPos;
	bool mIsActive;
};