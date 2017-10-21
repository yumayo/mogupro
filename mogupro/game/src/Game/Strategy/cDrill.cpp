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

cDrill::cDrill(const ci::vec3 _pos, const int _id, const DrillType type, const bool _ismyobject)
{
	drilltype = type;
	ismyobject = _ismyobject;

	switch (drilltype)
	{
	case Level1:
		scale = vec3(1, 1, 1);
		break;
	case Level2:
		scale = vec3(3, 3, 3);
		break;
	case Level3:
		scale = vec3(5, 5, 5);
		break;
	default:
		break;
	}

	machinescale = scale + vec3(2, 2, 2);
	pos = _pos + vec3(0, machinescale.y / 2.f, 0);
	beginpos = pos;
	id = _id;
	slope = vec3(scale.x, 0, scale.z);
	drillslope = 0.0f;
	createDrills();//ƒeƒXƒg
	root = Node::node::create();
	root->set_schedule_update();
	float a = 0.0f;
}
cDrill::~cDrill()
{

}
void cDrill::draw()
{
   
	drawBasket();

	drawCube(beginpos, machinescale, vec3(0, 0, 0), ColorA(0, 0, 1, 1));

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
		pos.y -= drillspeed*delta_time;
		Game::cFieldManager::getInstance()->blockBreak(pos, scale.z);
		collisionFieldGems();
		updateSlope(1.0f,delta_time);
		break;
	case Game::Strategy::cDrill::DRILLRETURN:
		y_rotate--;
		updateSlope(-1.0f,delta_time);
		pos.y += drillspeed*delta_time;;
		break;
	case Game::Strategy::cDrill::DRILLSTOP:
		break;
	default:
		break;
	}

	state = changeState();

	root->entry_update(delta_time);

	vec3 p = root->get_position_3d();
	

	moveGetGem(delta_time);

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

	for (int i = 0; i <GemManager->getGems().size(); i++)
	{
		if (GemManager->getGems()[i]->getIsDrillhit())continue;
		vec3 gempos = GemManager->getGems()[i]->getPos();
		vec3 gemscale = GemManager->getGems()[i]->getScale();

		AxisAlignedBox gem_aabb(gempos - vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f),
			gempos + vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f));

		if (STRM->isAABB(drill_aabb, gem_aabb))
		{
			getgems.push_back(GemManager->getGems()[i]);

			int index = getgems.size() - 1;

			getgems[index]->setSinRotate(atan2f(getgems[index]->getPos().z - pos.z, (getgems[index]->getPos().x - pos.x)));
			getgems[index]->setPutPos(pos);

			getgems[index]->root = Node::node::create();
			getgems[index]->root->set_schedule_update();


			getgems[index]->root->run_action(sequence::create(float_to::create((beginpos.y- getgems[index]->getPos().y)*0.1f, getgems[index]->getPos().y,
				beginpos.y + (scale.y + 1.f), [this, index](float t)
			{
				auto p = getgems[index]->root->get_position_3d();
				p.y = t;
				getgems[index]->root->set_position_3d(p);
			}), call_func::create([] {; })));
			GemManager->getGems()[i]->setIsDrillhit(true);


		/*	eracenum.push_back(GemManager->getGems()[i].getId());
			getgems.push_back(GemManager->getGems()[i]);
			getgems[getgems.size() - 1].setSinRotate(atan2f(getgems[getgems.size() - 1].getPos().z-pos.z, (getgems[getgems.size() - 1].getPos().x - pos.x)));
			getgems[getgems.size() - 1].setPutPos(pos);*/
		}

	}


	//GemManager->gemCountUp(GemManager->getGems()[i]);
}
void cDrill::drawBasket()
{
	float backetsize = machinescale.z / 20.f;
	ColorA pollcolor = ColorA(0.4,0.15,0.2);
	ColorA baketcolor = ColorA(0.7,0.35,0.4);
	float hulfscale = machinescale.x / 2.f;
	float hulfbacketsize = backetsize / 2.f;

	drawCube(beginpos + vec3(hulfscale, machinescale.y, 0), vec3(0,machinescale.y,backetsize), vec3(0, 0, 0), baketcolor);
	drawCube(beginpos + vec3(-hulfscale, machinescale.y, 0), vec3(0, machinescale.y, backetsize), vec3(0, 0, 0), baketcolor);
	drawCube(beginpos + vec3(0, machinescale.y, -hulfscale), vec3(backetsize, machinescale.y, 0), vec3(0, 0, 0), baketcolor);
	drawCube(beginpos + vec3(0, machinescale.y, hulfscale), vec3(backetsize, machinescale.y, 0), vec3(0, 0, 0), baketcolor);

	drawCube(beginpos + vec3(hulfscale, machinescale.y, 0), vec3(0, backetsize, machinescale.z), vec3(0, 0, 0), baketcolor);
	drawCube(beginpos + vec3(-hulfscale, machinescale.y, 0), vec3(0, backetsize, machinescale.z), vec3(0, 0, 0), baketcolor);


	drawCube(beginpos + vec3(-hulfscale, 1.5f*machinescale.y- hulfbacketsize, 0), vec3(0, backetsize, machinescale.z), vec3(0, 0, 0), baketcolor);
	drawCube(beginpos + vec3(hulfscale, 1.5f*machinescale.y - hulfbacketsize, 0), vec3(0, backetsize, machinescale.z), vec3(0, 0, 0), baketcolor);

	drawCube(beginpos + vec3(0, 1.5f*machinescale.y - hulfbacketsize,hulfscale), vec3(machinescale.z, backetsize, 0), vec3(0, 0, 0), baketcolor);
	drawCube(beginpos + vec3(0, machinescale.y - hulfbacketsize, hulfscale), vec3(machinescale.z, backetsize, 0), vec3(0, 0, 0), baketcolor);
	drawCube(beginpos + vec3(0, 1.5f*machinescale.y -hulfbacketsize, -hulfscale), vec3(machinescale.z, backetsize, 0), vec3(0, 0, 0), baketcolor);
	drawCube(beginpos + vec3(0, machinescale.y -hulfbacketsize,-hulfscale), vec3(machinescale.z, backetsize, 0), vec3(0, 0, 0), baketcolor);



	drawCube(beginpos + vec3(-hulfscale +hulfbacketsize, machinescale.y, -hulfscale + hulfbacketsize), vec3(backetsize, machinescale.y, backetsize), vec3(0, 0, 0), pollcolor);
	drawCube(beginpos + vec3(-hulfscale + hulfbacketsize, machinescale.y, hulfscale - hulfbacketsize), vec3(backetsize, machinescale.y, backetsize), vec3(0, 0, 0), pollcolor);
	drawCube(beginpos + vec3(hulfscale - hulfbacketsize, machinescale.y, hulfscale - hulfbacketsize), vec3(backetsize, machinescale.y, backetsize), vec3(0, 0, 0), pollcolor);
	drawCube(beginpos + vec3(hulfscale - hulfbacketsize, machinescale.y, -hulfscale + hulfbacketsize), vec3(backetsize, machinescale.y, backetsize), vec3(0, 0, 0), pollcolor);




	///drawCube(beginpos+vec3(0,machinescale.y,0), machinescale, vec3(0, 0, 0), ColorA(1, 0, 0, 1));

}
void cDrill::createDrills()
{

    DrillSlope drill;
    drill.pos = beginpos;
    drill.scale = scale;
    drill.rotate = float( drillslopes.size() )*17.f;
    drillslopes.push_back( DrillSlope( drill ) );

}
void cDrill::move(float delttime)
{
	pos.y -= drillspeed*delttime;
}

void cDrill::moveGetGem(const float delttime)
{
	console() << "delta: " << delttime << std::endl;
	for (int i = 0; i <getgems.size(); i++)
	{
		
		if (!(getgems[i]->getPos().y >= beginpos.y + (scale.y + 1.f))) {
			getgems[i]->root->entry_update(delttime);
			vec3 p = getgems[i]->root->get_position_3d();

			float rotate_speed = delttime*6.f;

			getgems[i]->setSinRotate(getgems[i]->getSinRotate() + rotate_speed);

			getgems[i]->setPos(vec3(getgems[i]->getPutPos().x + ((scale.x / 2.f) + 0.5f)*cos(getgems[i]->getSinRotate()),
				p.y,
				getgems[i]->getPutPos().z + ((scale.z / 2.f) + 0.5f)*sin(getgems[i]->getSinRotate())));
		
		}
	}

}
void cDrill::updateSlope(const float direction, float delttime)
{
	slope.y += drillspeed*direction*delttime;///////////

	for (int i = 0; i < drillslopes.size(); i++)
	{
		drillslopes[i].rotate += i % 2 == 0 ? 5.f*delttime : -5.f * delttime;
	}

	for (int i = 0; i < drillslopes.size(); i++)
	{
		drillslopes[i].pos -= vec3(0, drillspeed*direction*delttime, 0);
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
