#include <Game/Weapons/SubWeapon/cQuarry.h>
#include "cinder/gl/gl.h"
#include"Game/cFieldManager.h"
#include"Game/cStrategyManager.h"
#include"Game/cGemManager.h"
#include<Network.hpp>
#include"cinder\gl\scoped.h"
#include"Resource\TextureManager.h"
#include"Game\cClientAdapter.h"
#include"Game\cStrategyManager.h"
#include"cinder\Rand.h"
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
			cQuarry::cQuarry(const ci::vec3 pos, const int _objectid, const int playerid)
				: mSlopeAABB(pos, ci::vec3(1)), mMachineAABB(pos, ci::vec3(1)), mDrillAABB(pos, ci::vec3(1)) {
				ismyobject = (Game::cPlayerManager::getInstance()->getActivePlayerId() == playerid);
				mObjectId = _objectid;
				mPlayerId = playerid;
				mDrillScale = ci::vec3(1, 1, 1);
				setScale();
				mPos = pos;////machine本体
				mBeginDrillPos= mPos - ci::vec3(0, mScale.y / 2.f, 0) - ci::vec3(0, mDrillScale.y / 2.f, 0);
				mDrillPos = mBeginDrillPos;
				slope.pos = mPos;
				createSlope();
				root = Node::node::create();
				root->set_schedule_update();
				root->set_position_3d(mDrillPos);
				TEX->set("drill.png", "drill.png");
			}
			cQuarry::~cQuarry()
			{
				mMachineAABB.removeWorld();
				mDrillAABB.removeWorld();
				mSlopeAABB.removeWorld();

				for (auto& it : getgems) {
					ci::vec3 randpos = ci::randVec3()*(mScale.x/4.f);
					it->setPos(Game::cStrategyManager::getInstance()->getCannons()[int(cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getWhichTeam())]->getGemStorePos()+randpos);
				}

				Game::cStrategyManager::getInstance()->getCannons()[int(cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getWhichTeam())]->receiveQuarryGem(getgems);
			}
			void cQuarry::setup()
			{
				mMachineAABB.addWorld();
				mDrillAABB.addWorld();
				mSlopeAABB.addWorld();

				mMachineAABB.setSize(mScale);
				mDrillAABB.setSize(mDrillScale);
				mSlopeAABB.setSize(slope.scale);

				
			}
			void cQuarry::update(const float & delta_time)
			{
				switch (state)
				{
				case Game::Weapons::SubWeapon::cQuarry::DRILLMOVE:
					Drillmove(delta_time);
					Game::cFieldManager::getInstance()->blockBreak(mDrillPos, mDrillScale.x);
					collisionFieldGems();
					updateSlope(delta_time);
					mDrillrotate_y += 5.f*delta_time;
					break;
				case Game::Weapons::SubWeapon::cQuarry::DRILLRETURN:
					drillReturn(delta_time);
					updateSlope(delta_time);
					mDrillrotate_y += 5.f*delta_time;
					break;
				case Game::Weapons::SubWeapon::cQuarry::DRILLSTOP:
					break;
				default:
					break;
				}


				state = changeState();

				root->entry_update(delta_time);

				moveGetGem(delta_time);
				//ci::app::console() << mPos << std::endl;
			}
			void cQuarry::updateCollisionAfterUpdate(const float & delta_time)
			{

			}
			void cQuarry::draw()
			{
				ci::gl::ScopedGlslProg glsl(ci::gl::getStockShader(ci::gl::ShaderDef().color()));

				////drawBasket();

				drawMachine();
				drawSlope();
				drawDrill();
			}
			bool cQuarry::deleteThis()
			{
				return state == DrillState::DRILLSTOP;
			}
			void cQuarry::HitGem(const int _gemid)
			{

				if (!ismyobject)return;
			
				GemManager->breakeGemStone(_gemid);
				GemManager->getGemStone(_gemid)->setIsActive(false);

				/////////////////////////用修正AABBをでかくしておきます
				AxisAlignedBox drill_aabb(mDrillAABB.getPosition() - mDrillAABB.getSize()*2.f,
					mDrillAABB.getPosition() + mDrillAABB.getSize()*2.f);

			
				for (int i = 0; i < GemManager->getFragmentGems().size(); i++) {
		
					if (!GemManager->getFragmentGem(i)->isActive())continue;
				
					AxisAlignedBox fragmentgemaabb(GemManager->getFragmentGem(i)->getPos() - GemManager->getFragmentGem(i)->getScale()/ 2.f,
						GemManager->getFragmentGem(i)->getPos() + GemManager->getFragmentGem(i)->getScale() / 2.f);
					if (drill_aabb.intersects(fragmentgemaabb)) {
						
						getgems.push_back(GemManager->getFragmentGem(i));
					
						int index = getgems.size() - 1;
			
						getgems[index]->setIsRigid(false);
						getgems[index]->setSinRotate(atan2f(getgems[index]->getPos().z - mPos.z, (getgems[index]->getPos().x - mPos.x)));
						getgems[index]->setPutPos(vec3(mPos.x, mDrillPos.y, mPos.z));

						getgems[index]->node = Node::node::create();
						getgems[index]->node->set_schedule_update();

						getgems[index]->node->run_action(sequence::create(float_to::create((mPos.y - getgems[index]->getPos().y)*0.1f,
							getgems[index]->getPos().y, mPos.y + mScale.y/2.f+1.f, [this, index](float t)
						{
							auto p = getgems[index]->node->get_position_3d();
							p.y = t;
							getgems[index]->node->set_position_3d(p);
						}), call_func::create([] {; })));
					
						getgems[index]->setIsActive(false);
					}

				}

			

			
			}
			void cQuarry::setScale()
			{
				mScale = vec3(3, 3, 3);
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
				slope.pos.x = mDrillPos.x;
				slope.pos.y = ((mPos.y - mScale.y / 2.f) + (mDrillPos.y + mDrillScale.y / 2.f)) / 2.f;
				slope.pos.z = mDrillPos.z;
				slope.scale.y = ((mPos.y - mScale.y / 2.f) - (mDrillPos.y + mDrillScale.y / 2.f));
				slope.rotate_y += 5.f*delttime;
				mSlopeAABB.setPosition(slope.pos);
				mSlopeAABB.setSize(slope.scale);
			}
			void cQuarry::createSlope()
			{
				slope.pos = mPos;
				slope.rotate_y = 0.0f;
				slope.scale = ci::vec3(1, 0, 1);
			}
			void cQuarry::drawSlope()
			{
				float drawrate = 0.8f;
				drawCube(slope.pos, vec3(slope.scale.x*drawrate, std::max(slope.scale.y, 0.f), slope.scale.z*drawrate), vec3(0.0f, slope.rotate_y, 0.0f), ColorA(1.0f, 0.5f, 0.2f, 1.0f));
				drawCube(slope.pos, vec3(slope.scale.x*drawrate, std::max(slope.scale.y, 0.f), slope.scale.z*drawrate), vec3(0.0f, -slope.rotate_y, 0.0f), ColorA(0.2f, 0.5f, 1.0f, 1.0f));
			}

			void cQuarry::collisionFieldGems()
			{
				if (!ismyobject)return;

				AxisAlignedBox drill_aabb(mDrillAABB.getPosition()-mDrillAABB.getSize()/2.f,
					mDrillAABB.getPosition() + mDrillAABB.getSize() / 2.f);

				for (int i = 0; i < int(GemManager->getGemStones().size()); i++)
				{
					if (!GemManager->getGemStone(i)->isActive())continue;
					vec3 gempos = GemManager->getGemStone(i)->getPos();
					vec3 gemscale = GemManager->getGemStone(i)->getScale();

					AxisAlignedBox gem_aabb(gempos - vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f),
						gempos + vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f));

					if (STRM->isAABB(drill_aabb, gem_aabb))
					{
						/////////////////////////////通信
						
						//HitGem(GemManager->getGemStone(i)->getId())
					
						cClientAdapter::getInstance()->sendGetGemQuarry(mObjectId, GemManager->getGemStone(i)->getId());
						/////////////////////////////
						//cClientAdapter::getInstance()->sendGetGemQuarry(id, GemManager->getGems()[i]->getId());
					}
				}
			}
			void cQuarry::Drillmove(float delttime)
			{
				vec3 pos_ = root->get_position_3d();
				if (iseasingfinished)
				{
					iseasingfinished = false;
					root->run_action(sequence::create(ease<EaseOutExpo>::create(move_to::create(0.02F, getNextEasingPos())), call_func::create([this]
					{
						iseasingfinished = true;
						easingcount++;
					})));
				}
				mDrillPos = pos_;
				mDrillAABB.setPosition(mDrillPos);
			}

			void cQuarry::drillReturn(float delttime)
			{
				mDrillPos.x = mPos.x;
				mDrillPos.y += delttime*10.0f;
				mDrillPos.z = mPos.z;
			}
		
			void cQuarry::drawDrill()
			{
				float rate = mDrillScale.y / 5.f;
				drawCube(mDrillPos - vec3(0, 2, 0)*rate, vec3(rate*1.f, rate*1.f, rate*1.f), vec3(0, mDrillrotate_y*5.f, 0), ColorA(1, 1, 1, 1));
				drawCube(mDrillPos - vec3(0, 1, 0)*rate, vec3(rate*1.f, rate*1.f, rate*1.f), vec3(0, mDrillrotate_y, 0), ColorA(0, 0, 0, 1));
				drawCube(mDrillPos, vec3(rate*3.f, rate*1.f, rate*3.f), vec3(0, mDrillrotate_y*3.f, 0), ColorA(1, 0, 0, 1));
				drawCube(mDrillPos + vec3(0, 1, 0)*rate, vec3(rate*4.f, rate*1.f, rate*4.f), vec3(0, mDrillrotate_y*3.f, 0), ColorA(0, 0, 0, 1));
				drawCube(mDrillPos + vec3(0, 2, 0)*rate, vec3(rate*4.f, rate*1.f, rate*4.f), vec3(0, mDrillrotate_y*-3.f, 0), ColorA(1, 1, 0, 1));
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

			void cQuarry::moveGetGem(const float delttime)
			{
				for (int i = 0; i < int(getgems.size()); i++)
				{
					if (getgems[i]->getPos().y < mPos.y + mScale.y / 2.f + 1.f) {
						getgems[i]->node->entry_update(delttime);
						vec3 p = getgems[i]->node->get_position_3d();

						float rotate_speed = delttime*6.f;

						getgems[i]->setSinRotate(getgems[i]->getSinRotate() + rotate_speed);

						getgems[i]->setPos(vec3(getgems[i]->getPutPos().x + (mScale.x / 2.f)*cos(getgems[i]->getSinRotate()),
							p.y,
							getgems[i]->getPutPos().z + (mScale.z / 2.f)*sin(getgems[i]->getSinRotate())));
						
					}
					else {
					
						//getgems[i]->setPos(ci::vec3(getgems[i]->getPos().x, mPos.y + mScale.y, getgems[i]->getPos().z));
					}
					
				}
			}

			Game::Weapons::SubWeapon::cQuarry::DrillState cQuarry::changeState()
			{
				switch (state)
				{
				case Game::Weapons::SubWeapon::cQuarry::DRILLMOVE:
					if ((mPos.y - mDrillPos.y) >= 60.f) {
						return DrillState::DRILLRETURN;
					}
					return DrillState::DRILLMOVE;

				case Game::Weapons::SubWeapon::cQuarry::DRILLRETURN:
					if ((mPos.y - mDrillPos.y) <= 0.5f) {
						//位置調整
						return DrillState::DRILLSTOP;
					}
					return DrillState::DRILLRETURN;
				case Game::Weapons::SubWeapon::cQuarry::DRILLSTOP:
					return DrillState::DRILLSTOP;
				default:
					break;
				}
				return DrillState::DRILLMOVE;
			}

			void cQuarry::drawMachine()
			{
				cinder::gl::ScopedTextureBind a(TEX->get("drill.png"));
				gl::ScopedGlslProg shader(gl::getStockShader(gl::ShaderDef().texture()));
				drawCube(mPos, vec3(mScale.x, mScale.y, mScale.z), vec3(0, 0, 0), ColorA(1, 1, 1, 1));
			}
			ci::vec3 cQuarry::getNextEasingPos()
			{
				int num = mScale.x;
				vec3 buf;
				int z_direction = easingcount / (num*num) % 2 == 0 ? 1 : -1;
				int direction = 1 * z_direction;
				if ((easingcount % (num*num) / num) % 2 == 1) {
					direction = -1 * z_direction;
				}

				if (easingcount / (num*num) % 2 == 0) {
					buf = vec3(mPos.x - direction*((num - 1) / 2) + direction*(easingcount%num),
						mBeginDrillPos.y - (easingcount / (num*num)),
						mPos.z - ((num - 1) / 2) + z_direction*((easingcount % (num*num)) / num));
				}
				else {
					buf = vec3(mPos.x - direction*((num - 1) / 2) + direction*(easingcount%num),
						mBeginDrillPos.y - (easingcount / (num*num)),
						mPos.z + ((num - 1) / 2) - ((easingcount % (num*num)) / num));
				}

				return buf;
			}
		}
	}
}
