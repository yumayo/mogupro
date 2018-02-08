#include <Game/UI/cTargetCannon.h>
#include <Resource/cImageManager.h>
#include <Node/renderer.hpp>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cFieldManager.h>
#include <Node/action.hpp>
namespace Game
{
namespace UI
{
using namespace cinder;
CREATE_CPP(cTargetCannon, Player::Team team)
{
	CREATE(cTargetCannon, team);
}
bool cTargetCannon::init(Player::Team team)
{
	icon = add_child(Node::Renderer::sprite::create(Resource::IMAGE[team == Player::Red ? "in_game/red_cannon_icon.png" : "in_game/blue_cannon_icon.png"]));
	allow = add_child(Node::Renderer::sprite::create( Resource::IMAGE[team == Player::Red ? "in_game/red_circle.png" : "in_game/blue_circle.png"] ) );
	set_schedule_update( );
	return true;
}
void cTargetCannon::update(float delta)
{
	auto& cannons = cStrategyManager::getInstance()->getCannons();
	auto& cannon = cannons.at(cPlayerManager::getInstance()->getActivePlayerTeamId());
	auto cannonPos = cannon->getGemStorePos();

	auto const& camera = CameraManager::cCameraManager::getInstance()->getCamera();

	auto playerPos = cPlayerManager::getInstance()->getActivePlayer()->getPos();

	auto cameraPos = camera.getEyePoint();

	auto cameraVec = camera.getViewDirection();
	cameraVec = ci::normalize(cameraVec);

	ci::vec3 targetVec = cannonPos - cameraPos;
	auto targetLength = glm::length(targetVec);
	targetVec = ci::normalize(targetVec);

	ci::Frustum frustum(camera);
	std::vector<ci::Plane> planes;
	planes.emplace_back(frustum.getPlane(ci::Frustum::NEAR));
	planes.emplace_back(frustum.getPlane(ci::Frustum::FAR));
	planes.emplace_back(frustum.getPlane(ci::Frustum::LEFT));
	planes.emplace_back(frustum.getPlane(ci::Frustum::RIGHT));
	planes.emplace_back(frustum.getPlane(ci::Frustum::TOP));
	planes.emplace_back(frustum.getPlane(ci::Frustum::BOTTOM));

	std::vector<ci::vec3> planesNormal;
	for (auto p : planes)
	{
		planesNormal.emplace_back(p.getNormal());
	}

	bool initHit = frustum.intersects(cannonPos);
	bool isHit = initHit;
	while (false == isHit)
	{
		{
			auto d = frustum.getPlane(ci::Frustum::NEAR).distance(cannonPos);
			if (d < 0)
			{
				cannonPos -= planesNormal[ci::Frustum::NEAR] * d;
				isHit = true;
			}
		}
		{
			auto d = frustum.getPlane(ci::Frustum::LEFT).distance(cannonPos);
			if (d < 0)
			{
				cannonPos -= planesNormal[ci::Frustum::LEFT] * d;
				isHit = true;
			}
		}
		{
			auto d = frustum.getPlane(ci::Frustum::RIGHT).distance(cannonPos);
			if (d < 0)
			{
				cannonPos -= planesNormal[ci::Frustum::RIGHT] * d;
				isHit = true;
			}
		}
		{
			auto d = frustum.getPlane(ci::Frustum::TOP).distance(cannonPos);
			if (d < 0)
			{
				cannonPos -= planesNormal[ci::Frustum::TOP] * d;
				isHit = true;
			}
		}
		{
			auto d = frustum.getPlane(ci::Frustum::BOTTOM).distance(cannonPos);
			if (d < 0)
			{
				cannonPos -= planesNormal[ci::Frustum::BOTTOM] * d;
				isHit = true;
			}
		}
	}

	auto screenPos = camera.worldToScreen(cannonPos, app::getWindowWidth(), app::getWindowHeight());
	
	// UI‚É‚©‚Ô‚ç‚È‚¢‚æ‚¤‚É‚·‚é
	auto drawScreenPos = glm::clamp( screenPos, vec2( 250, 150 ) + icon->get_content_size( ) / 2.0F, vec2( app::getWindowSize( ) ) - vec2( 250, 0 ) - icon->get_content_size( ) / 2.0F );
	// –{—ˆ‚ÌêŠ‚É‰ñ“]‚³‚¹‚éB
	ci::vec2 vec = screenPos - ci::app::getWindowCenter();
	float angle = atan2(vec.y, vec.x);

	remove_all_actions();

	if (!initHit)
	{
		run_action(Node::Action::move_to::create(0.1F, drawScreenPos));
	}
	else
	{
		set_position(drawScreenPos);
	}
	allow->set_rotation(angle);
}
}
}
