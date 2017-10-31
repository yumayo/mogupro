#pragma once
#include"Game/Strategy/cStrategyObjectBase.h"
#include"Game/cGemManager.h"
#include"Node\node.h"
#include"Node\action.hpp"
namespace Game
{
	namespace Strategy
	{

		class cCannon : public cStrategyObjectBase
		{
		public:
			cCannon(const ci::vec3 _pos,const float _direction, const bool _ismyobject);
			~cCannon();
			void draw() override;
			void update(const float& delta_time) override;
			void setup() override;
			bool DeleteThis() override;
			void setField(const ci::vec3 pos) override;
			const ci::vec3 SCALE = ci::vec3(20, 15, 40);
		private:
			float direction;
			Utility::hardptr<Node::node> root;
		};
	}
}
