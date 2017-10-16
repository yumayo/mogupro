#include<Game/Strategy/cDrill.h>
#include "cinder/gl/gl.h"
#include"Game/cFieldManager.h"
#include"Game/cStrategyManager.h"
#include"Game/cGemManager.h"


using namespace ci;
using namespace ci::app;

using namespace Node;

using namespace Node::Action;
namespace Game
{
namespace Strategy
{

cDrill::cDrill(const ci::vec3 _pos, const ci::vec3 _scale, const int _id)
{


	machinescale = _scale + vec3(2, 2, 2);
	pos = _pos + vec3(0, machinescale.y / 2.f, 0);
	scale = _scale;
	beginpos = pos;
	id = _id;
	slope = vec3(scale.x, 0, scale.z);
	drillslope = 0.0f;
	createDrills();//ƒeƒXƒg

	root = Node::node::create();
	root->set_schedule_update();
	float a = 0;

	root->run_action(float_to::create(2.0f, 1.0f, 10.0f, [this](float t)
	{
		auto p = root->get_position_3d();
		p.y = t;
		root->set_position_3d(p);
	}));
}
cDrill::~cDrill()
{

}
void cDrill::draw()
{
    drawCube( beginpos, machinescale, vec3( 0, 0, 0 ), ColorA( 0, 0, 1, 1 ) );
    //drawCube(pos, scale, vec3(0,0, 0), ColorA(1, 0, 0));

    float drawrate = 0.8f;
    drawCube( beginpos + vec3( 0, -slope.y / 2.f, 0 ), vec3( scale.x*drawrate, std::max( slope.y - float( 1.f*scale.y ), 0.f ), scale.z*drawrate ), vec3( 0, y_rotate, 0 ), ColorA( 1, 0.5, 0.2, 1 ) );
    drawCube( beginpos + vec3( 0, -slope.y / 2.f, 0 ), vec3( scale.x*drawrate, std::max( slope.y - float( 1.f*scale.y ), 0.f ), scale.z*drawrate ), vec3( 0, -y_rotate, 0 ), ColorA( 0, 0, 0, 1 ) );

    for (int i = 0; i < drillslopes.size(); i++)
    {
        if (i == 0)
        {
            float rate = drillslopes[i].scale.y / 5.f;
            drawCube( drillslopes[i].pos - vec3( 0, 2, 0 )*rate, vec3( rate*1.f, rate*1.f, rate*1.f ), vec3( 0, drillslopes[i].rotate*5.f, 0 ), ColorA( 1, 1, 1, 1 ) );
            drawCube( drillslopes[i].pos - vec3( 0, 1, 0 )*rate, vec3( rate*1.f, rate*1.f, rate*1.f ), vec3( 0, drillslopes[i].rotate, 0 ), ColorA( 0, 0, 0, 1 ) );
            drawCube( drillslopes[i].pos, vec3( rate*3.f, rate*1.f, rate*3.f ), vec3( 0, drillslopes[i].rotate*3.f, 0 ), ColorA( 1, 0, 0, 1 ) );
            drawCube( drillslopes[i].pos + vec3( 0, 1, 0 )*rate, vec3( rate*4.f, rate*1.f, rate*4.f ), vec3( 0, drillslopes[i].rotate*3.f, 0 ), ColorA( 0, 0, 0, 1 ) );
            drawCube( drillslopes[i].pos + vec3( 0, 2, 0 )*rate, vec3( rate*4.f, rate*1.f, rate*4.f ), vec3( 0, drillslopes[i].rotate*-3.f, 0 ), ColorA( 1, 1, 0, 1 ) );
        }
        else
        {
            drawCube( drillslopes[i].pos, drillslopes[i].scale, vec3( 0, drillslopes[i].rotate, 0 ), ColorA( 0, 0, 0, 1 ) );
            drawCube( drillslopes[i].pos, drillslopes[i].scale, vec3( 0, -drillslopes[i].rotate, 0 ), ColorA( 1, 0.5, 0.2, 1 ) );
        }

    }

    gl::color( ColorA( 1, 1, 1, 1 ) );
}
void cDrill::update( const float & delta_time )
{
 //   y_rotate++;
 //   move();

 //   //Game::Field::cFieldManager::getInstance()->blockBreak(pos, scale.z);

 //   //if ((beginpos.y - pos.y) >= (float(drillslopes.size())*scale.y)) {
 //   //	createDrills();
 //   //}

	//updateSlope();
  
	switch (state)
	{
	case Game::Strategy::cDrill::DRILLMOVE:
		y_rotate++;
		pos.y -= drillspeed;
		Game::cFieldManager::getInstance()->blockBreak(pos, scale.z);
		updateSlope(1.0f);
		break;
	case Game::Strategy::cDrill::DRILLRETURN:
		updateSlope(-1.0f);
		pos.y += drillspeed;
		break;
	case Game::Strategy::cDrill::DRILLSTOP:
		break;
	default:
		break;
	}
	root->entry_update(delta_time);


}
void cDrill::setup()
{

}
bool cDrill::DeleteThis()
{
	return state == DrillState::DRILLSTOP;
}

void cDrill::setField(const ci::vec3 pos)
{
}

void cDrill::drawCube( const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color )
{
    gl::pushModelView();
    gl::translate( pos );
    gl::rotate( rotate.x, vec3( 1, 0, 0 ) );
    gl::rotate( rotate.y, vec3( 0, 1, 0 ) );
    gl::rotate( rotate.z, vec3( 0, 0, 1 ) );
    gl::scale( size );
    gl::color( color );
    gl::drawCube( vec3( 0, 0, 0 ), vec3( 1, 1, 1 ) );
    gl::popModelView();
}
void cDrill::collisionFieldGems()
{
	AxisAlignedBox drill_aabb(pos - vec3(float(scale.x) / 2.f, float(scale.y) / 2.f, float(scale.z) / 2.f),
		pos + vec3(float(scale.x) / 2.f, float(scale.y) / 2.f, float(scale.z) / 2.f));

	int n = GemManager->getGems().size();
	for (int i = 0; i <n; i++)
	{
		vec3 gempos = GemManager->getGems()[i].getPos();
		vec3 gemscale = GemManager->getGems()[i].getScale();

		AxisAlignedBox gem_aabb(gempos - vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f),
			gempos + vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f));
		if (STRM->isAABB(drill_aabb, gem_aabb))
		{
			getgems.push_back(GemManager->getGems()[i]);
			GemManager->gemDelete(i);//ID
		}

	}
	//GemManager->gemCountUp(GemManager->getGems()[i]);
}
void cDrill::createDrills()
{

    DrillSlope drill;
    drill.pos = beginpos;
    drill.scale = scale;
    drill.rotate = float( drillslopes.size() )*17.f;
    drillslopes.push_back( DrillSlope( drill ) );

}
void cDrill::move()
{
    pos.y -= drillspeed;
}
void cDrill::moveGetGem()
{
	for (int i = 0; i <getgems.size(); i++)
	{
		/*vec3 gempos = getgems[i].getPos();
		vec3 gemscale = getgems[i].getScale();*/

		 



	}
}
void cDrill::updateSlope(const float direction)
{
	slope.y += drillspeed*direction;///////////

	for (int i = 0; i < drillslopes.size(); i++)
	{
		drillslopes[i].rotate += i % 2 == 0 ? 5 : -5;
	}

	for (int i = 0; i < drillslopes.size(); i++)
	{
		drillslopes[i].pos -= vec3(0, direction*drillspeed, 0);
	}
}

void cDrill::setting(const ci::vec3 _beginpos)
{
	beginpos = _beginpos;
	pos = beginpos;
}

Strategy::cDrill::DrillState cDrill::changeState()
{
	switch (state)
	{
	case Game::Strategy::cDrill::DRILLMOVE:
		if ((beginpos.y - pos.y) >= 60.f) {
			return DrillState::DRILLRETURN;
		}
		return DrillState::DRILLMOVE;

	case Game::Strategy::cDrill::DRILLRETURN:
		if ((beginpos.y - pos.y) <= 0.5f) {
			setting(beginpos);
			return DrillState::DRILLSTOP;
		}
		return DrillState::DRILLRETURN;
	case Game::Strategy::cDrill::DRILLSTOP:

		return DrillState::DRILLSTOP;
	default:
		break;
	}
}

}

}
