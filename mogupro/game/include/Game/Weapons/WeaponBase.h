#pragma once
#include <cinder/Ray.h>
#include <cinder/app/App.h>
#include <cinder/AxisAlignedBox.h>
namespace Game {
	namespace Weapon {
		
		class WeaponBase {
		protected:
			float attack;
			float range;
			ci::Ray ray[3];
			ci::AxisAlignedBox aabb;
			bool is_attack;
			bool push;
			bool press;
			bool pull;
		public:
			WeaponBase() {}
			virtual void pushCall(bool push) {
				this->push = push;
			}
			virtual void pressCall(bool press) {
				this->press = press;
			}
			virtual void pullCall(bool pull) {
				this->pull = pull;
			}
			virtual void setup() = 0;
			virtual void update(const float& delta_time) = 0;
			virtual void draw() = 0;
		};
	}
}