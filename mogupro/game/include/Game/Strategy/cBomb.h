#pragma once
#include"Game/Strategy/cStrategyObjectBase.h"
#include"Node\node.h"
#include"Node\action.hpp"
#include"Collision\cRigidBody.h"
#include"Collision\cAABBCollider.h"
namespace Game
{
	namespace Strategy
	{

		class cBomb : public cStrategyObjectBase
		{
		public:
			cBomb(const ci::vec3 _pos, const ci::vec3 _speed, const ci::vec3 _scale, const bool _ismyobject);
			~cBomb();
			void draw() override;
			void update(const float& delta_time) override;
			void updateCollisionAfterUpdate(const float& delta_time)override;
			void setup() override;
			bool DeleteThis() override;
			void setField(const ci::vec3 pos) override;
			const ci::vec3 SCALE = ci::vec3(20, 15, 40);
		private:
			Utility::hardptr<Node::node> root;
			ci::vec3 speed;
			Collision::cRigidBody rb;
			Collision::cAABBCollider aabb;
			bool onaabb = false;
			int aabbcount = 0;
		};
	}
}