#pragma once
#include <cinder/Ray.h>
#include <cinder/app/App.h>
#include <cinder/AxisAlignedBox.h>
namespace Game {
	namespace Weapon {
		
		class WeaponBase {
		protected:
			ci::vec3 center_pos;
			ci::vec3 player_vec;
			ci::vec3 debug_intersects;
			float attack;
			float range;
			bool hit;
			ci::Ray ray;
			ci::AxisAlignedBox aabb;
		public:
			WeaponBase() {}
			//RayとAABBのSetは子クラスで各々やってね
			ci::Ray getRay() {
				return ray;
			}
			ci::AxisAlignedBox getAABB() {
				return aabb;
			}
			void setDebugIntersects(ci::vec3 pos) {
				debug_intersects = pos;
			}
			//毎フレーム呼ぶ
			void setPlayerTransform(ci::vec3 pos,ci::vec3 vec) {
				center_pos = pos;
				player_vec = vec;
			}
			virtual void setup() = 0;
			virtual void update(const float& delta_time) = 0;
			virtual void draw() = 0;
		};
	}
}