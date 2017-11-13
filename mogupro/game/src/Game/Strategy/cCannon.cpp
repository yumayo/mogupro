#include<Game/Strategy/cCannon.h>
#include "cinder/gl/gl.h"
#include"Game/cStrategyManager.h"
#include"Game/cGemManager.h"
#include<Network.hpp>
#include"cinder\gl\scoped.h"
#include"Game\cClientAdapter.h"
#include"Game\cPlayerManager.h"
using namespace ci;
using namespace ci::app;

using namespace Node;

using namespace Node::Action;
namespace Game
{
	namespace Strategy
	{
		cCannon::cCannon(const ci::vec3 _pos, const float _direction, const bool _ismyobject)
		{
			mPos = _pos;
			scale = vec3(8, 8, 8);
			id = 0;
			ismyobject = _ismyobject;
			direction = _direction;

		}
		cCannon::~cCannon()
		{
		}

		void cCannon::draw()
		{
			float rate = scale.x;
			///////////////“y‘ä
			vec3 dpos = mPos + vec3(0, -rate*0.45f, 0);
			STRM->drawCube(dpos, vec3(rate,rate*0.1f,rate), vec3(0, 0, 0), ColorA(0.5, 0.5,0.5, 1));
			////////////////
			
			///////////////ƒLƒƒƒmƒ“
			STRM->drawCube(mPos+vec3(0,0,direction*rate*0.15f),vec3(rate*0.5f,rate*1.5f,rate*0.5f*direction), vec3(45*direction, 0, 0), ColorA(0.2, 0.2, 0.2, 1));
			////////////////

			///////////////ƒTƒCƒh
			STRM->drawCube(mPos+vec3(rate*0.3f,-rate*0.2f, -rate*0.15f*direction), vec3(rate*0.1f, rate*0.5f, rate*0.5), vec3(0, 0, 0), ColorA(0, 0, 1, 1));
			STRM->drawCube(mPos+vec3(-rate*0.3f,-rate*0.2f, -rate*0.15f*direction), vec3(rate*0.1f, rate*0.5f, rate*0.5), vec3(0, 0, 0), ColorA(0, 0, 1, 1));
			////////////////
			//STRM->drawCube(mPos, scale, vec3(0, 0, 0), ColorA(0.2, 0.2, 0.2, 1));
		}

		void cCannon::update(const float & delta_time)
		{

		}

		void cCannon::updateCollisionAfterUpdate(const float & delta_time)
		{
		}

		void cCannon::setup()
		{

		}

		bool cCannon::DeleteThis()
		{
			return false;
		}

		void cCannon::setField(const ci::vec3 pos)
		{

		}

	}
}
