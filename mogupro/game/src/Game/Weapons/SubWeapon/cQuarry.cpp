#include <Game/Weapons/SubWeapon/cQuarry.h>
#include "cinder/gl/gl.h"
#include"Game/cFieldManager.h"
#include"Game/cStrategyManager.h"
#include"Game/cGemManager.h"
#include<Network.hpp>
#include"cinder\gl\scoped.h"
#include"Resource\TextureManager.h"
#include"Game\cClientAdapter.h"
using namespace ci;
using namespace ci::app;

using namespace Node;

using namespace Node::Action;
namespace Game
{
	namespace Weapons
	{
		namespace SubWeapon
		{
			cQuarry::cQuarry(const ci::vec3 pos, const int _objectid, const int playerid, const DrillType type)
				: mSlopeAABB(mPos, ci::vec3(1)), mMachineAABB(mPos, ci::vec3(1)), mDrillAABB(mPos, ci::vec3(1)) {
				drilltype = type;
				ismyobject = (Game::cPlayerManager::getInstance()->getActivePlayerId() == playerid);
				mObjectId = _objectid;
				mPlayerId = playerid;
				setScale(drilltype);
				mPos = pos;////machine–{‘Ì
				createSlope();
				root = Node::node::create();
				root->set_schedule_update();
				root->set_position_3d(mPos);
				TEX->set("drill.png", "drill.png");
			}
			cQuarry::~cQuarry()
			{

			}
			void cQuarry::setup()
			{
			}
			void cQuarry::update(const float & delta_time)
			{
				//switch (state)
				//{
				//case Game::Weapons::SubWeapon::cQuarry::DRILLMOVE:
				//	move(delta_time);
				//	Game::cFieldManager::getInstance()->blockBreak(mPos, mScale.z);
				//	collisionFieldGems();
				//	updateSlope(1.0f, delta_time);
				//	y_rotate++;
				//	break;
				//case Game::Weapons::SubWeapon::cQuarry::DRILLRETURN:
				//	y_rotate--;
				//	updateSlope(-1.0f, delta_time);
				//	mPos.y += drillspeed*delta_time;;
				//	break;
				//case Game::Weapons::SubWeapon::cQuarry::DRILLSTOP:
				//	break;
				//default:
				//	break;
				//}


				state = changeState();

				root->entry_update(delta_time);

				moveGetGem(delta_time);
			}
			void cQuarry::updateCollisionAfterUpdate(const float & delta_time)
			{
			}
			void cQuarry::draw()
			{
				//ci::gl::ScopedGlslProg glsl(ci::gl::getStockShader(ci::gl::ShaderDef().color()));

				////drawBasket();

				//drawMachine();

				//float drawrate = 0.8f;
				//drawCube(vec3(mPos.x, (beginpos.y + mPos.y) / 2.f, mPos.z), vec3(scale.x*drawrate, std::max(slope.y - float(1.f*scale.y), 0.f), scale.z*drawrate), vec3(0.0f, y_rotate, 0.0f), ColorA(1.0f, 0.5f, 0.2f, 1.0f));
				//drawCube(vec3(mPos.x, (beginpos.y + mPos.y) / 2.f, mPos.z), vec3(scale.x*drawrate, std::max(slope.y - float(1.f*scale.y), 0.f), scale.z*drawrate), vec3(0.0f, -y_rotate, 0.0f), ColorA(0.0f, 0.f, 0.f, 1));

				//float rate = drillslopes.scale.y / 5.f;
				//drawCube(mPos - vec3(0, 2, 0)*rate, vec3(rate*1.f, rate*1.f, rate*1.f), vec3(0, drillslopes.rotate*5.f, 0), ColorA(1, 1, 1, 1));
				//drawCube(mPos - vec3(0, 1, 0)*rate, vec3(rate*1.f, rate*1.f, rate*1.f), vec3(0, drillslopes.rotate, 0), ColorA(0, 0, 0, 1));
				//drawCube(mPos, vec3(rate*3.f, rate*1.f, rate*3.f), vec3(0, drillslopes.rotate*3.f, 0), ColorA(1, 0, 0, 1));
				//drawCube(mPos + vec3(0, 1, 0)*rate, vec3(rate*4.f, rate*1.f, rate*4.f), vec3(0, drillslopes.rotate*3.f, 0), ColorA(0, 0, 0, 1));
				//drawCube(mPos + vec3(0, 2, 0)*rate, vec3(rate*4.f, rate*1.f, rate*4.f), vec3(0, drillslopes.rotate*-3.f, 0), ColorA(1, 1, 0, 1));
			}
			bool cQuarry::deleteThis()
			{
				return state == DrillState::DRILLSTOP;
			}
			void cQuarry::HitGem(const int _gemid)
			{
				//if (!ismyobject)return;

				//getgems.push_back(GemManager->FindGem(_gemid));

				//int index = getgems.size() - 1;

				//getgems[index]->setSinRotate(atan2f(getgems[index]->getPos().z - beginpos.z, (getgems[index]->getPos().x - beginpos.x)));
				//getgems[index]->setPutPos(vec3(beginpos.x, mPos.y, beginpos.z));

				//getgems[index]->root = Node::node::create();
				//getgems[index]->root->set_schedule_update();

				//getgems[index]->root->run_action(sequence::create(float_to::create((beginpos.y - getgems[index]->getPos().y)*0.1f, getgems[index]->getPos().y,
				//	beginpos.y + (scale.y + 1.f), [this, index](float t)
				//{
				//	auto p = getgems[index]->root->get_position_3d();
				//	p.y = t;
				//	getgems[index]->root->set_position_3d(p);
				//}), call_func::create([] {; })));

				//GemManager->getGems()[_gemid]->setIsDrillhit(true);
			}
			void cQuarry::setScale(const DrillType _type)
			{
				switch (_type)
				{
				case Level1:
					mScale = vec3(1, 1, 1);
					break;
				case Level2:
					mScale = vec3(3, 3, 3);
					break;
				case Level3:
					mScale = vec3(5, 5, 5);
					break;
				default:
					mScale = vec3(1, 1, 1);
					break;
				}
			}
			void cQuarry::drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color)
			{
				gl::pushModelView();
				gl::translate(pos);
				gl::rotate(rotate.x, vec3(1, 0, 0));
				gl::rotate(rotate.y, vec3(0, 1, 0));
				gl::rotate(rotate.z, vec3(0, 0, 1));
				gl::scale(size);
				gl::color(color);
				gl::drawCube(vec3(0, 0, 0), vec3(1, 1, 1));
				gl::popModelView();
			}

			void cQuarry::updateSlope(float delttime)
			{
				slope.pos.y = ((mPos.y - mScale.y / 2.f) + (mDrillPos.y + mDrillScale.y / 2.f)) / 2.f;
				slope.scale.y = ((mPos.y - mScale.y / 2.f) - (mDrillPos.y + mDrillScale.y / 2.f)) / 2.f;
				slope.rotate_y += 5.f*delttime;
			}
			void cQuarry::createSlope()
			{
				slope.pos = mPos;
				slope.scale = ci::vec3(1, 0, 1);
			}
			void cQuarry::setupDrill()
			{
			}
			void cQuarry::collisionFieldGems()
			{
				//if (!ismyobject)return;

				//AxisAlignedBox drill_aabb(mPos - vec3(float(scale.x) / 2.f, float(scale.y) / 2.f, float(scale.z) / 2.f),
				//	mPos + vec3(float(scale.x) / 2.f, float(scale.y) / 2.f, float(scale.z) / 2.f));

				//for (int i = 0; i < int(GemManager->getGems().size()); i++)
				//{
				//	if (GemManager->getGems()[i]->getIsDrillhit())continue;
				//	vec3 gempos = GemManager->getGems()[i]->getPos();
				//	vec3 gemscale = GemManager->getGems()[i]->getScale();

				//	AxisAlignedBox gem_aabb(gempos - vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f),
				//		gempos + vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f));

				//	if (STRM->isAABB(drill_aabb, gem_aabb))
				//	{
				//		cClientAdapter::getInstance()->sendGetGemQuarry(id, GemManager->getGems()[i]->getId());
				//	}
				//}
			}
			void cQuarry::Drillmove(float delttime)
			{

			}
			void cQuarry::drawBasket()
			{

				//float backetsize = machinescale.z / 20.f;
				//ColorA pollcolor = ColorA(0.4f, 0.15f, 0.2f);
				//ColorA baketcolor = ColorA(0.7f, 0.35f, 0.4f);
				//float hulfscale = machinescale.x / 2.f;
				//float hulfbacketsize = backetsize / 2.f;

				//drawCube(beginpos + vec3(hulfscale, machinescale.y, 0), vec3(0, machinescale.y, backetsize), vec3(0, 0, 0), baketcolor);
				//drawCube(beginpos + vec3(-hulfscale, machinescale.y, 0), vec3(0, machinescale.y, backetsize), vec3(0, 0, 0), baketcolor);
				//drawCube(beginpos + vec3(0, machinescale.y, -hulfscale), vec3(backetsize, machinescale.y, 0), vec3(0, 0, 0), baketcolor);
				//drawCube(beginpos + vec3(0, machinescale.y, hulfscale), vec3(backetsize, machinescale.y, 0), vec3(0, 0, 0), baketcolor);

				//drawCube(beginpos + vec3(hulfscale, machinescale.y, 0), vec3(0, backetsize, machinescale.z), vec3(0, 0, 0), baketcolor);
				//drawCube(beginpos + vec3(-hulfscale, machinescale.y, 0), vec3(0, backetsize, machinescale.z), vec3(0, 0, 0), baketcolor);


				//drawCube(beginpos + vec3(-hulfscale, 1.5f*machinescale.y - hulfbacketsize, 0), vec3(0, backetsize, machinescale.z), vec3(0, 0, 0), baketcolor);
				//drawCube(beginpos + vec3(hulfscale, 1.5f*machinescale.y - hulfbacketsize, 0), vec3(0, backetsize, machinescale.z), vec3(0, 0, 0), baketcolor);

				//drawCube(beginpos + vec3(0, 1.5f*machinescale.y - hulfbacketsize, hulfscale), vec3(machinescale.z, backetsize, 0), vec3(0, 0, 0), baketcolor);
				//drawCube(beginpos + vec3(0, machinescale.y - hulfbacketsize, hulfscale), vec3(machinescale.z, backetsize, 0), vec3(0, 0, 0), baketcolor);
				//drawCube(beginpos + vec3(0, 1.5f*machinescale.y - hulfbacketsize, -hulfscale), vec3(machinescale.z, backetsize, 0), vec3(0, 0, 0), baketcolor);
				//drawCube(beginpos + vec3(0, machinescale.y - hulfbacketsize, -hulfscale), vec3(machinescale.z, backetsize, 0), vec3(0, 0, 0), baketcolor);



				//drawCube(beginpos + vec3(-hulfscale + hulfbacketsize, machinescale.y, -hulfscale + hulfbacketsize), vec3(backetsize, machinescale.y, backetsize), vec3(0, 0, 0), pollcolor);
				//drawCube(beginpos + vec3(-hulfscale + hulfbacketsize, machinescale.y, hulfscale - hulfbacketsize), vec3(backetsize, machinescale.y, backetsize), vec3(0, 0, 0), pollcolor);
				//drawCube(beginpos + vec3(hulfscale - hulfbacketsize, machinescale.y, hulfscale - hulfbacketsize), vec3(backetsize, machinescale.y, backetsize), vec3(0, 0, 0), pollcolor);
				//drawCube(beginpos + vec3(hulfscale - hulfbacketsize, machinescale.y, -hulfscale + hulfbacketsize), vec3(backetsize, machinescale.y, backetsize), vec3(0, 0, 0), pollcolor);




				///drawCube(beginpos+vec3(0,machinescale.y,0), machinescale, vec3(0, 0, 0), ColorA(1, 0, 0, 1));
			}

			//void cQuarry::move(float delttime)
			//{
			//	//vec3 pos_ = root->get_position_3d();
			//	//if (iseasingfinished)
			//	//{
			//	//	iseasingfinished = false;
			//	//	root->run_action(sequence::create(ease<EaseOutExpo>::create(move_to::create(0.2F, getNextEasingPos())), call_func::create([this]
			//	//	{
			//	//		iseasingfinished = true;
			//	//		easingcount++;
			//	//	})));
			//	//}
			//	//mPos = pos_;
			//}
			void cQuarry::moveGetGem(const float delttime)
			{
				/*for (int i = 0; i < int(getgems.size()); i++)
				{

					if (!(getgems[i]->getPos().y >= beginpos.y + (mcale.y + 1.f))) {
						getgems[i]->root->entry_update(delttime);
						vec3 p = getgems[i]->root->get_position_3d();

						float rotate_speed = delttime*6.f;

						getgems[i]->setSinRotate(getgems[i]->getSinRotate() + rotate_speed);

						getgems[i]->setPos(vec3(getgems[i]->getPutPos().x + ((scale.x / 2.f) + 0.5f)*cos(getgems[i]->getSinRotate()),
							p.y,
							getgems[i]->getPutPos().z + ((scale.z / 2.f) + 0.5f)*sin(getgems[i]->getSinRotate())));

					}
				}*/
			}

			Game::Weapons::SubWeapon::cQuarry::DrillState cQuarry::changeState()
			{
	/*			switch (state)
				{
				case Game::Weapons::SubWeapon::cQuarry::DRILLMOVE:
					if ((beginpos.y - mPos.y) >= 60.f) {
						return DrillState::DRILLRETURN;
					}
					return DrillState::DRILLMOVE;

				case Game::Weapons::SubWeapon::cQuarry::DRILLRETURN:
					if ((beginpos.y - mPos.y) <= 0.5f) {
						setting(beginpos);
						return DrillState::DRILLSTOP;
					}
					return DrillState::DRILLRETURN;
				case Game::Weapons::SubWeapon::cQuarry::DRILLSTOP:

					return DrillState::DRILLSTOP;
				default:
					break;
				}*/
				return DrillState::DRILLMOVE;
			}

			void cQuarry::drawMachine()
			{
				//cinder::gl::ScopedTextureBind a(TEX->get("drill.png"));
				//gl::ScopedGlslProg shader(gl::getStockShader(gl::ShaderDef().texture()));
				//drawCube(beginpos, vec3(machinescale.x*1.1f, machinescale.y, machinescale.z*1.1f), vec3(0, 0, 0), ColorA(1, 1, 1, 1));
			}
			ci::vec3 cQuarry::getNextEasingPos()
			{
				//int num = int(scale.x) + 2;
				//vec3 buf;
				//int z_direction = easingcount / (num*num) % 2 == 0 ? 1 : -1;
				//int direction = 1 * z_direction;
				//if ((easingcount % (num*num) / num) % 2 == 1) {
				//	direction = -1 * z_direction;
				//}
				//buf = vec3(beginpos.x - direction*((num - 1) / 2) + direction*(easingcount%num), beginpos.y - (easingcount / (num*num)), beginpos.z - ((num - 1) / 2) + z_direction*((easingcount % (num*num)) / num));
				//if (easingcount / (num*num) % 2 == 0) {

				//}
				//else {
				//	buf = vec3(beginpos.x - direction*((num - 1) / 2) + direction*(easingcount%num), beginpos.y - (easingcount / (num*num)), beginpos.z + ((num - 1) / 2) - ((easingcount % (num*num)) / num));
				//}

				//return buf;
			}
		}
	}
}
