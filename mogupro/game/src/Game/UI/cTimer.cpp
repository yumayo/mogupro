#include <Game/UI/cTimer.h>
#include <Resource/cImageManager.h>
#include <Node/renderer.hpp>
#include <Game/cGameManager.h>
#include <Resource/cSoundManager.h>
#include <Node/action.hpp>
using namespace Node::Action;
using namespace cinder;
namespace Game
{
namespace UI
{
bool cTimer::init(cinder::vec2 contentSize)
{
	Node::Renderer::sprite::init(Resource::IMAGE["gameMainUI/timer.png"]);
	this->set_schedule_update();
	this->set_anchor_point(vec2(0.5F, 0.0F));
	this->set_position(contentSize * vec2(0.5F, 0.0F));
	this->set_pivot(vec2(0.5F, 0.125F));
	letter = this->add_child(Node::Renderer::letter::create("AMEMUCHIGOTHIC-06.ttf", 36.0F));
	letter->set_space(-9.0F);
	letter->set_text("12:34");
	letter->set_color(ColorA(0, 0, 0));

	STATE_GENERATE(sMac, start);
	STATE_GENERATE(sMac, lastOneMinutes);
	STATE_GENERATE(sMac, countDown);
	STATE_GENERATE(sMac, finish);

	start->join(lastOneMinutes, [](auto)
	{
		return cGameManager::getInstance()->getLeftBattleTimef() <= 60.0F;
	});
	lastOneMinutes->onStateIn = [this](auto)
	{
		if (lastOneMinutesCall) lastOneMinutesCall();
		Resource::SE["in_game/last_one_minutes.wav"].play();
		letter->set_color( ColorA(0.75F, 0.75F, 0) );
		run_action(repeat_times::create(sequence::create(call_func::create([this] 
		{
			letter->run_action( sequence::create(
				ease<ci::EaseOutCubic>::create(scale_by::create(0.2F, vec2(0.2F))),
				ease<ci::EaseOutCubic>::create(scale_by::create(0.2F, vec2(-0.2F)))
			));
		}), delay::create(1.0F)), 60));
	};
	lastOneMinutes->join(countDown, [](auto)
	{
		return cGameManager::getInstance()->getLeftBattleTimef() <= 10.0F;
	});
	countDown->onStateIn = [this, countDown, finish](auto)
	{
		run_action( sequence::create( repeat_times::create(sequence::create(call_func::create([this] 
		{
			Resource::SE["in_game/count_down.wav"].play(); 
			if (countDownCall) countDownCall(std::to_string((int)cGameManager::getInstance()->getLeftBattleTimef()));
		}), delay::create(1.0F)), 10), call_func::create([countDown, finish]
		{
			countDown->join(finish, [](auto) {return true; });
		})));
	};

	finish->onStateIn = [](auto)
	{
		Resource::SE["in_game/finish.wav"].play();
	};

	sMac.setEntryNode(start);

	return true;
}
void cTimer::update(float delta)
{
	sMac.update(delta);
	letter->set_text(cGameManager::getInstance()->getLeftBattleTime());
}
}
}
