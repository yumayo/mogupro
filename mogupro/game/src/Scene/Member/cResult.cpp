#include <Scene/Member/cResult.h>
#include <Game/cGameManager.h>
#include <Node/renderer.hpp>
#include <Game/Player/cPlayer.h>
#include <CameraManager/cCameraManager.h>
#include <Game/Field/FieldData.h>
#include <Utility/cInput.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
#include <Resource/cImageManager.h>
#include <Game/cPlayerManager.h>
#include <Resource/cSoundManager.h>
using namespace Node::Renderer;
using namespace cinder;
using namespace Node::Action;
namespace Scene
{
namespace Member
{
using namespace cinder;
class ButtonNextNode : public Node::Renderer::sprite
{
public:
	CREATE_H(ButtonNextNode) { CREATE(ButtonNextNode); }
	bool init()
	{
		__super::init( Resource::IMAGE["in_game/buttonb.png"] );
		set_schedule_update( );
		return true;
	}
	void update( float ) override
	{
		if (ENV->pushKey())
		{
			Scene::cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
		}
	}
};
class CapsuleNode : public Node::node
{
	geom::Capsule capsule;
	float radius = 0.0F, length = 0.0F;
public:
	CREATE_H(CapsuleNode, float radius, float length) { CREATE(CapsuleNode, radius, length); }
	bool init(float radius, float length)
	{
		set_radius(radius);
		set_length(length);
		return true;
	}
	void set_length(float value)
	{
		length = value;
		capsule.length(value);
		capsule.center(vec3(radius) + vec3(0.0F, length / 2.0F, 0.0F));
		set_content_size_3d(vec3(radius * 2.0F) + vec3(0.0F, length, 0.0F));
	}
	float get_length()
	{
		return length;
	}
	void set_radius(float value)
	{
		radius = value;
		capsule.radius(value);
		capsule.center(vec3(radius) + vec3(0.0F, length / 2.0F, 0.0F));
		set_content_size_3d(vec3(radius * 2.0F) + vec3(0.0F, length, 0.0F));
	}
	float get_radius()
	{
		return radius;
	}
	void render()
	{
		gl::draw(capsule);
	}
public:
	class radius_to : public Node::Action::finite_time_action
	{
	public:
		CREATE_H(radius_to, float duration, float radius)
		{
			CREATE(radius_to, duration, radius);
		}
		bool init(float duration, float radius)
		{
			finite_time_action::init(duration);
			_radius = radius;
			return true;
		}
	protected:
		void setup() override
		{
			if (auto target = _target.dynamicptr<CapsuleNode>())
			{
				auto radius = target->get_radius();
				_start_radius = radius;
			}
		}
		void step(float t) override
		{
			t = clamp(t, 0.0F, 1.0F);
			auto const to = _radius;
			auto const from = _start_radius;
			auto const temp = ease_liner(t, from, to);
			if (auto target = _target.dynamicptr<CapsuleNode>())
			{
				target->set_radius(temp);
			}
		}
		void restart() override
		{
			finite_time_action::restart();
			setup();
		}
	protected:
		float _start_radius = 0.0F;
		float _radius = 0.0F;
	};
	class length_to : public Node::Action::finite_time_action
	{
	public:
		CREATE_H(length_to, float duration, float length)
		{
			CREATE(length_to, duration, length);
		}
		bool init(float duration, float length)
		{
			finite_time_action::init(duration);
			_length = length;
			return true;
		}
	protected:
		void setup() override
		{
			if (auto target = _target.dynamicptr<CapsuleNode>())
			{
				auto length = target->get_length();
				_start_length = length;
			}
		}
		void step(float t) override
		{
			t = clamp(t, 0.0F, 1.0F);
			auto const to = _length;
			auto const from = _start_length;
			auto const temp = ease_liner(t, from, to);
			if (auto target = _target.dynamicptr<CapsuleNode>())
			{
				target->set_length(temp);
			}
		}
		void restart() override
		{
			finite_time_action::restart();
			setup();
		}
	protected:
		float _start_length = 0.0F;
		float _length = 0.0F;
	};
};
class TorusNode : public Node::node
{
	geom::Torus torus;
	float major = 0.0F, minor = 0.0F;
public:
	CREATE_H(TorusNode, float major, float minor) { CREATE(TorusNode, major, minor); }
	bool init(float major, float minor)
	{
		set_radius(major, minor);
		return true;
	}
	void set_radius(float major, float minor)
	{
		this->major = major;
		this->minor = minor;
		torus.radius(major, minor);
	}
	struct { float major, minor; } get_radius()
	{
		return { major, minor };
	}
	void render()
	{
		gl::draw(torus);
	}
public:
	class radius_to : public Node::Action::finite_time_action
	{
	public:
		CREATE_H(radius_to, float duration, float major, float minor)
		{
			CREATE(radius_to, duration, major, minor);
		}
		bool init(float duration, float major, float minor)
		{
			finite_time_action::init(duration);
			_major = major;
			_minor = minor;
			return true;
		}
	protected:
		void setup() override
		{
			if (auto target = _target.dynamicptr<TorusNode>())
			{
				auto radius = target->get_radius();
				_start_major = radius.major;
				_start_minor = radius.minor;
			}
		}
		void step(float t) override
		{
			t = clamp(t, 0.0F, 1.0F);
			float major, minor;
			{
				auto const to = _major;
				auto const from = _start_major;
				major = ease_liner(t, from, to);
			}
			{
				auto const to = _minor;
				auto const from = _start_minor;
				minor = ease_liner(t, from, to);
			}
			if (auto target = _target.dynamicptr<TorusNode>())
			{
				target->set_radius(major, minor);
			}
		}
		void restart() override
		{
			finite_time_action::restart();
			setup();
		}
	protected:
		float _start_major = 0.0F;
		float _start_minor = 0.0F;
		float _major = 0.0F;
		float _minor = 0.0F;
	};
};
void cResult::setup()
{
	root = Node::node::create();
	root->set_schedule_update();
	root->set_content_size(app::getWindowSize() - ivec2(100, 100));
	root->set_scale(vec2(1, -1));
	root->set_position(root->get_content_size() * vec2(-0.5F, 0.5F));

	root3d = Node::node::create();
	root3d->set_schedule_update();

	auto* gm = Game::cGameManager::getInstance();

	CAMERA->setScheduleUpdate(false);
	CAMERA->setCameraMode(::CameraManager::FPS);

	upNode = root3d->add_child(Node::node::create());
	upNode->set_position_3d(vec3(0, 1, 0));

	Resource::BGM["result/cannon_power.wav"].play();

	{
		std::vector<std::string> redPlayerName;
		bool win = gm->winTeam() == Game::Player::Red;
		for (int i = 0; i < 3; ++i)
		{
			redPlayerName.emplace_back(u8"‚à‚®‚ç" + std::to_string(i));
		}
		auto& n = win ? winBoard : loseBoard;
		n = root->add_child(createScoreBoard(
			Game::Player::Red,
			win,
			redPlayerName,
			gm->getRedTeamAppendGemData(),
			gm->getRedTeamKillData(),
			gm->getRedTeamDeathData()));
		auto size_x = n->get_content_size().x * 2;
		n->set_position(root->get_content_size() * (win ? vec2(0, 1) : vec2(1, 1)) + vec2(win ? -size_x : size_x, 0.0F));
		n->set_anchor_point(win ? vec2(0, 1) : vec2(1, 1));
	}
	{
		std::vector<std::string> bluePlayerName;
		bool win = gm->winTeam() == Game::Player::Blue;
		for (int i = 4; i < 7; ++i)
		{
			bluePlayerName.emplace_back(u8"‚à‚®‚ç" + std::to_string(i));
		}
		auto& n = win ? winBoard : loseBoard;
		n = root->add_child(createScoreBoard(
			Game::Player::Blue,
			win,
			bluePlayerName,
			gm->getBlueTeamAppendGemData(),
			gm->getBlueTeamKillData(),
			gm->getBlueTeamDeathData()));
		auto size_x = n->get_content_size().x * 2;
		n->set_position(root->get_content_size() * (win ? vec2(0, 1) : vec2(1, 1)) + vec2(win ? -size_x : size_x, 0.0F));
		n->set_anchor_point(win ? vec2(0, 1) : vec2(1, 1));
	}

	STATE_GENERATE(sMac, power_in);
	STATE_GENERATE(sMac, burst);
	STATE_GENERATE(sMac, sky);
	STATE_GENERATE(sMac, score_board);

	auto redPowerRoot = root3d->add_child(Node::node::create());
	redPowerRoot->set_position_3d(Game::Field::WORLD_SIZE * vec3(0.5F, 1.0F, 0.0F) + vec3(0, 5, 7));
	redPowerRoot->set_rotation(-glm::pi<float>() / 6.0F + glm::pi<float>());
	redPowerRoot->set_axis(vec3(1, 0, 0));
	redCapsuleNode = redPowerRoot->add_child(CapsuleNode::create(0.8F, 0.0F));
	redCapsuleNode->set_position_3d(vec3(0.0F, 0.0F, -1.0F));
	redCapsuleNode->set_rotation(-glm::pi<float>() / 2.0F);
	redCapsuleNode->set_axis(vec3(1, 0, 0));
	redCapsuleNode->set_color(ColorA(1, 0, 0));
	redCapsuleNode->set_anchor_point_3d(vec3(0.5F, 0.0F, 0.5F));
	redCapsuleNode->set_pivot_3d(vec3(0.5F, 1.0F, 0.5F));

	auto bluePowerRoot = root3d->add_child(Node::node::create());
	bluePowerRoot->set_position_3d(Game::Field::WORLD_SIZE * vec3(0.5F, 1.0F, 1.0F) + vec3(0, 5, -7));
	bluePowerRoot->set_rotation(glm::pi<float>() / 6.0F);
	bluePowerRoot->set_axis(vec3(1, 0, 0));
	blueCapsuleNode = bluePowerRoot->add_child(CapsuleNode::create(0.8F, 0.0F));
	blueCapsuleNode->set_position_3d(vec3(0.0F, 0.0F, -1.0F));
	blueCapsuleNode->set_rotation(-glm::pi<float>() / 2.0F);
	blueCapsuleNode->set_axis(vec3(1, 0, 0));
	blueCapsuleNode->set_color(ColorA(0, 0, 1));
	blueCapsuleNode->set_anchor_point_3d(vec3(0.5F, 0.0F, 0.5F));
	blueCapsuleNode->set_pivot_3d(vec3(0.5F, 1.0F, 0.5F));

	switch (Game::cPlayerManager::getInstance()->getActivePlayerTeamId())
	{
	case Game::Player::Red:
	{
		eyeNode = redPowerRoot->add_child(Node::node::create());
		tarNode = redPowerRoot->add_child(Node::node::create());
		{
			auto const NUM = std::max(gm->getResult().second, 1);
			float const ONE_TIME = 4.8F / NUM;
			bluePowerRoot->run_action(repeat_times::create(sequence::create(call_func::create([this, bluePowerRoot, ONE_TIME] {bluePowerRoot->add_child(createPowerTorus(ONE_TIME))->set_color(ColorA(0, 0, 1)); }), delay::create(ONE_TIME)), NUM));
		}
		{
			auto const NUM = std::max(gm->getResult().first, 1);
			float const ONE_TIME = 4.8F / NUM;
			redPowerRoot->run_action(sequence::create(repeat_times::create(sequence::create(call_func::create([this, redPowerRoot, ONE_TIME] {redPowerRoot->add_child(createPowerTorus(ONE_TIME))->set_color(ColorA(1, 0, 0)); }), delay::create(ONE_TIME)), NUM),
				call_func::create([this, power_in, burst, sky]
			{
				power_in->join(burst, [](auto n) { return true; });

				eyeNode->remove_all_actions();
				tarNode->remove_from_parent();
				tarNode = redCapsuleNode->add_child(Node::node::create());

				blueCapsuleNode->run_action(CapsuleNode::length_to::create(1.0F, Game::Field::WORLD_SIZE.z - 37));

				redCapsuleNode->run_action(sequence::create(CapsuleNode::length_to::create(1.0F, Game::Field::WORLD_SIZE.z - 37), call_func::create([this, burst, sky]
				{
					burst->join(sky, [](auto n) { return true; });
					upNode->run_action(ease<ci::EaseOutCubic>::create(move_to::create(1.0F, vec3(0, 0, 1))));
				})));
			})));
		}
		break;
	}
	case Game::Player::Blue:
	{
		eyeNode = bluePowerRoot->add_child(Node::node::create());
		tarNode = bluePowerRoot->add_child(Node::node::create());
		{
			auto const NUM = std::max(gm->getResult().first, 1);
			float const ONE_TIME = 4.8F / NUM;
			redPowerRoot->run_action(repeat_times::create(sequence::create(call_func::create([this, bluePowerRoot, ONE_TIME] {bluePowerRoot->add_child(createPowerTorus(ONE_TIME))->set_color(ColorA(0, 0, 1)); }), delay::create(ONE_TIME)), NUM));
		}
		{
			auto const NUM = std::max(gm->getResult().second, 1);
			float const ONE_TIME = 4.8F / NUM;
			bluePowerRoot->run_action(sequence::create(repeat_times::create(sequence::create(call_func::create([this, bluePowerRoot, ONE_TIME] {bluePowerRoot->add_child(createPowerTorus(ONE_TIME))->set_color(ColorA(0, 0, 1)); }), delay::create(ONE_TIME)), NUM),
				call_func::create([this, power_in, burst, sky]
			{
				power_in->join(burst, [](auto n) { return true; });

				eyeNode->remove_all_actions();
				tarNode->remove_from_parent();
				tarNode = blueCapsuleNode->add_child(Node::node::create());

				redCapsuleNode->run_action(CapsuleNode::length_to::create(1.0F, Game::Field::WORLD_SIZE.z - 37));

				blueCapsuleNode->run_action(sequence::create(CapsuleNode::length_to::create(1.0F, Game::Field::WORLD_SIZE.z - 37), call_func::create([this, burst, sky]
				{
					burst->join(sky, [](auto n) { return true; });
					upNode->run_action(ease<ci::EaseOutCubic>::create(move_to::create(1.0F, vec3(0, 0, -1))));
				})));
			})));
		}
		break;
	}
	default:
		break;
	}

	eyeNode->set_position_3d(vec3(vec3(3, 0, -8)));
	eyeNode->run_action(move_by::create(10.0F, vec3(5, 0, -7)));
	tarNode->set_position_3d(vec3(0, 0, 0));
	tarNode->run_action(move_by::create(10.0F, vec3(0, 0, -2)));

	burst->onStateIn = [this](auto m)
	{
		
	};

	sky->onStateIn = [this, sky, score_board](auto m)
	{
		auto mat = eyeNode->get_world_matrix_3d();
		eyeNode->remove_from_parent();
		eyeNode = root3d->add_child(Node::node::create());
		eyeNode->set_matrix_3d(mat);
		eyeNode->run_action(sequence::create(ease<ci::EaseOutCubic>::create(move_to::create(1.0F, Game::Field::WORLD_SIZE * vec3(0.5F, 3.0F, 0.5F))), call_func::create([sky, score_board]
		{
			sky->join(score_board, [](auto) { return true; });
		})));
	};

	score_board->onStateIn = [this](auto)
	{
		Resource::BGM["result/cannon_power.wav"].fadeout(2.0F, 0.0F);

		auto move_x = winBoard->get_content_size().x * 2;
		winBoard->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, vec2(move_x, 0))));
		loseBoard->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, vec2(-move_x, 0))));

		root->run_action(sequence::create(delay::create(2.0F), call_func::create([this]
		{
			auto n = root->add_child(ButtonNextNode::create());
			n->set_position(root->get_content_size() * vec2(1, 1));
			n->run_action(repeat_forever::create(sequence::create(
				ease<ci::EaseOutCubic>::create(scale_to::create(1.0F, vec2(1.1F))),
				ease<ci::EaseOutCubic>::create(scale_to::create(1.0F, vec2(1.0F)))
			)));
		})));
	};

	sMac.setEntryNode(power_in);
}
void cResult::shutDown()
{
	Resource::BGM["result/cannon_power.wav"].stop();
}
void cResult::resize()
{
}
void cResult::update(float deltaTime)
{
	auto* gm = Game::cGameManager::getInstance();

	auto eyeMat = eyeNode->get_world_matrix_3d();
	vec3 eye(eyeMat[3][0], eyeMat[3][1], eyeMat[3][2]);
	auto tarMat = tarNode->get_world_matrix_3d();
	vec3 tar(tarMat[3][0], tarMat[3][1], tarMat[3][2]);
	auto upMat = upNode->get_world_matrix_3d();
	vec3 up(upMat[3][0], upMat[3][1], upMat[3][2]);
	CAMERA->lookAt(eye, tar, up);

	sMac.update(deltaTime);
	root3d->entry_update(deltaTime);
	root->entry_update(deltaTime);
}
void cResult::draw()
{
	root3d->entry_render(ci::mat4());
}
void cResult::draw2D()
{
	root->entry_render(ci::mat4());
}
hardptr<Node::node> cResult::createScoreBoard(int team, bool win, std::vector<std::string> playerNameData, std::vector<int> pointData, std::vector<int> killData, std::vector<int> deathData)
{
	auto board = Node::Renderer::sprite::create(Resource::IMAGE[win ? "result/win_board.png" : "result/lose_board.png"]);
	board->set_color(team == Game::Player::Red ? ColorA(1, 0, 0) : ColorA(0, 0, 1));
	board->set_pivot(vec2(0, 0));
	for (int i = 0; i < 3; ++i)
	{
		auto scr = board->add_child(Node::Renderer::sprite::create(Resource::IMAGE["result/bar.png"]));
		scr->set_anchor_point(vec2(0, 0));
		scr->set_pivot(vec2(0, 0));
		scr->set_position(vec2(17, 138 + i * 70));

		auto gem = scr->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 32));
		gem->set_text(std::to_string(pointData[i]));
		gem->set_anchor_point(vec2(0, 0));
		gem->set_position(vec2(403, 12));

		auto kill = scr->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 18));
		kill->set_text(std::to_string(killData[i]));
		kill->set_anchor_point(vec2(0, 0));
		kill->set_position(vec2(551, 8));

		auto death = scr->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 18));
		death->set_text(std::to_string(deathData[i]));
		death->set_anchor_point(vec2(0, 0));
		death->set_position(vec2(551, 34));

		auto name = scr->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 36));
		name->set_text(playerNameData[i]);
		name->set_anchor_point(vec2(0, 0));
		name->set_position(vec2(93, 12));
	}
	return board;
}
hardptr<Node::node> cResult::createPowerTorus( float time )
{
	float one = time / 2.0F;

	auto t = TorusNode::create(2.2F, 2.0F);
	t->set_position_3d(vec3(0, 0, 2.5F));
	t->set_rotation(-glm::pi<float>() / 2.0F);
	t->run_action(sequence::create(move_to::create(one, vec3(0, 0, -0.5F)), spawn::create(move_to::create(one, vec3(0, 0, -1.0F)), TorusNode::radius_to::create(one, 0.8F, 0.0F)), remove_self::create()));
	t->set_axis(vec3(1, 0, 0));
	return t;
}
}
}
