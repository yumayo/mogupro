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
#include <Network/cMatchingMemberManager.h>
#include <Network/cUDPClientManager.h>
using namespace Node::Renderer;
using namespace cinder;
using namespace Node::Action;
namespace Scene
{
namespace Member
{
using namespace cinder;
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
		virtual void setup() override
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
		virtual void restart() override
		{
			finite_time_action::restart();
			setup();
		}
	protected:
		float _start_radius = 0.0F;
		float _radius = 0.0F;
	};
	class radius_by : public radius_to
	{
	public:
		CREATE_H(radius_by, float duration, float radius)
		{
			CREATE(radius_by, duration, radius);
		}
		bool init(float duration, float radius)
		{
			radius_to::init(duration, radius);
			_init_radius = radius;
			return true;
		}
	protected:
		void setup() override
		{
			radius_to::setup();
			_radius = _init_radius + _start_radius;
		}
		void restart() override
		{
			radius_to::restart();
			setup();
		}
	protected:
		float _init_radius = 0.0F;
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
		virtual void setup() override
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
		virtual void restart() override
		{
			finite_time_action::restart();
			setup();
		}
	protected:
		float _start_length = 0.0F;
		float _length = 0.0F;
	};
	class length_by : public length_to
	{
	public:
		CREATE_H(length_by, float duration, float length)
		{
			CREATE(length_by, duration, length);
		}
		bool init(float duration, float length)
		{
			length_to::init(duration, length);
			_init_length = length;
			return true;
		}
	protected:
		void setup() override
		{
			length_to::setup();
			_length = _init_length + _start_length;
		}
		void restart() override
		{
			length_to::restart();
			setup();
		}
	protected:
		float _init_length = 0.0F;
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
class PlayerNode : public Node::node
{
	//アニメーション
	Game::Animation::cAnimation animation;
	cinder::gl::TextureRef texture;
public:
	CREATE_H(PlayerNode, int winTeam, int team) { CREATE(PlayerNode, winTeam, team); }
	bool init(int winTeam, int team)
	{
		set_schedule_update();
		switch (team)
		{
		case Game::Player::Red:
			texture = Resource::IMAGE["in_game/UV_mogura_red.jpg"];
			break;
		case Game::Player::Blue:
			texture = Resource::IMAGE["in_game/UV_mogura_blue.jpg"];
			break;
		default:
			break;
		}
		if (winTeam == team)
		{
			animation.create("mogura_win", false, true);
			animation.animationChange("mogura_win");
		}
		else
		{
			animation.create("mogura_lose", false, true);
			animation.animationChange("mogura_lose");
		}
		return true;
	}
	void update( float delta ) override
	{
		animation.update(delta);
	}
	void render() override
	{
		ci::gl::ScopedDepth depth(true);
		ci::gl::ScopedGlslProg glsl(ci::gl::getStockShader(ci::gl::ShaderDef().texture()) );
		ci::gl::ScopedTextureBind tex(texture);
		animation.draw();
	}
};
const float CANNON_POWER_HALF_LENGTH = ( Game::Field::WORLD_SIZE.z - 8 * 2 ) / 2.0F;
void cResult::setup()
{
	// 0番の人はサーバーにゲームが終了したことを通知します。
	if (Game::cPlayerManager::getInstance()->getActivePlayerId() == 0)
	{
		Network::cUDPClientManager::getInstance()->send(new Network::Packet::Request::cReqGameEnd());
	}

	// リザルトは長いのでサーバーからの応答がなくても自動で切断させません。
	Network::cUDPClientManager::getInstance()->setDontClose(true);

	root = Node::node::create();
	root->set_schedule_update();
	root->set_content_size(app::getWindowSize());
	root->set_scale(vec2(1, -1));
	root->set_position(root->get_content_size() * vec2(-0.5F, 0.5F));

	rootUI = root->add_child(Node::node::create());
	rootUI->set_content_size(app::getWindowSize() - ivec2(100, 100));
	rootUI->set_position(vec2(100, 100) / 2.0F);

	root3d = Node::node::create();
	root3d->set_schedule_update();

	auto* gm = Game::cGameManager::getInstance();

	CAMERA->setScheduleUpdate(false);
	CAMERA->setCameraMode(::CameraManager::FPS);

	upNode = root3d->add_child(Node::node::create());
	upNode->set_position_3d(vec3(0, 1, 0));

	Resource::BGM["result/cannon_power.wav"].play();
	Resource::BGM["result/cannon_power.wav"].setGain( 0.7F );

	{
		std::vector<std::string> redPlayerName;
		bool win = gm->winTeam() == Game::Player::Red;
		for (int i = 0; i < 3; ++i)
		{
			redPlayerName.emplace_back(Game::cPlayerManager::getInstance()->getPlayer(i)->playerName);
		}
		auto& n = win ? winBoard : loseBoard;
		n = rootUI->add_child(createScoreBoard(
			Game::Player::Red,
			win,
			redPlayerName,
			gm->getRedTeamAppendGemData(),
			gm->getRedTeamKillData(),
			gm->getRedTeamDeathData()));
		auto size_x = n->get_content_size().x * 2;
		n->set_position(rootUI->get_content_size() * (win ? vec2(0, 1) : vec2(1, 1)) + vec2(win ? -size_x : size_x, 0.0F));
		n->set_anchor_point(win ? vec2(0, 1) : vec2(1, 1));
	}
	{
		std::vector<std::string> bluePlayerName;
		bool win = gm->winTeam() == Game::Player::Blue;
		for (int i = 4; i < 7; ++i)
		{
			bluePlayerName.emplace_back(Game::cPlayerManager::getInstance()->getPlayer(i)->playerName);
		}
		auto& n = win ? winBoard : loseBoard;
		n = rootUI->add_child(createScoreBoard(
			Game::Player::Blue,
			win,
			bluePlayerName,
			gm->getBlueTeamAppendGemData(),
			gm->getBlueTeamKillData(),
			gm->getBlueTeamDeathData()));
		auto size_x = n->get_content_size().x * 2;
		n->set_position(rootUI->get_content_size() * (win ? vec2(0, 1) : vec2(1, 1)) + vec2(win ? -size_x : size_x, 0.0F));
		n->set_anchor_point(win ? vec2(0, 1) : vec2(1, 1));
	}

	STATE_GENERATE(sMac, power_in);
	STATE_GENERATE(sMac, burst);
	STATE_GENERATE(sMac, sky);
	STATE_GENERATE(sMac, enemy);
	STATE_GENERATE(sMac, my);
	STATE_GENERATE(sMac, shake);
	STATE_GENERATE(sMac, judge);
	STATE_GENERATE(sMac, score_board);
	STATE_GENERATE(sMac, win_lose);
	STATE_GENERATE(sMac, bar);
	STATE_GENERATE(sMac, thankyouforplaying);
	STATE_GENERATE(sMac, blackout);
	STATE_GENERATE(sMac, gototitle);

	auto redPowerRoot = root3d->add_child(Node::node::create());
	redPowerRoot->set_position_3d(Game::Field::WORLD_SIZE * vec3(0.5F, 1.0F, 0.0F) + vec3(0, 5.5F, 7));
	redPowerRoot->set_rotation(/*-glm::pi<float>() / 6.0F +*/ glm::pi<float>());
	redPowerRoot->set_axis(vec3(1, 0, 0));
	redCapsuleNode = redPowerRoot->add_child(CapsuleNode::create(0.8F, 0.0F));
	redCapsuleNode->set_rotation(-glm::pi<float>() / 2.0F);
	redCapsuleNode->set_axis(vec3(1, 0, 0));
	redCapsuleNode->set_color(ColorA(1, 0, 0));
	redCapsuleNode->set_anchor_point_3d(vec3(0.5F, 0.0F, 0.5F));
	redCapsuleNode->set_pivot_3d(vec3(0.5F, 1.0F, 0.5F));

	auto bluePowerRoot = root3d->add_child(Node::node::create());
	bluePowerRoot->set_position_3d(Game::Field::WORLD_SIZE * vec3(0.5F, 1.0F, 1.0F) + vec3(0, 5.5F, -7));
	bluePowerRoot->set_rotation(/*glm::pi<float>() / 6.0F +*/ 0.0F);
	bluePowerRoot->set_axis(vec3(1, 0, 0));
	blueCapsuleNode = bluePowerRoot->add_child(CapsuleNode::create(0.8F, 0.0F));
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
		eyeNode->set_position_3d(vec3(vec3(3, 0, -8)));
		eyeNode->run_action(move_by::create(10.0F, vec3(5, 0, -7)));
		tarNode = redPowerRoot->add_child(Node::node::create());
		tarNode->set_position_3d(vec3(0, 0, 0));
		tarNode->run_action(move_by::create(10.0F, vec3(0, 0, -2)));
		{
			auto const NUM = std::max(gm->getResult().second / 20, 1);
			float const ONE_TIME = 4.8F / NUM;
			bluePowerRoot->run_action(repeat_times::create(sequence::create(call_func::create([this, bluePowerRoot, ONE_TIME] {bluePowerRoot->add_child(createPowerTorus(ONE_TIME))->set_color(ColorA(0, 0, 1)); }), delay::create(ONE_TIME)), NUM));
		}
		{
			auto const NUM = std::max(gm->getResult().first / 20, 1);
			float const ONE_TIME = 4.8F / NUM;
			redPowerRoot->run_action(sequence::create(repeat_times::create(sequence::create(call_func::create([this, redPowerRoot, ONE_TIME] {redPowerRoot->add_child(createPowerTorus(ONE_TIME))->set_color(ColorA(1, 0, 0)); }), delay::create(ONE_TIME)), NUM),
				call_func::create([this, power_in, burst, sky]
			{
				power_in->join(burst, [](auto n) { return true; });

				eyeNode->remove_all_actions();
				tarNode->remove_from_parent();
				tarNode = redCapsuleNode->add_child(Node::node::create());

				blueCapsuleNode->run_action(CapsuleNode::length_to::create(1.0F, CANNON_POWER_HALF_LENGTH));

				redCapsuleNode->run_action(sequence::create(CapsuleNode::length_to::create(1.0F, CANNON_POWER_HALF_LENGTH), call_func::create([this, burst, sky]
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
		eyeNode->set_position_3d(vec3(vec3(3, 0, -8)));
		eyeNode->run_action(move_by::create(10.0F, vec3(5, 0, -7)));
		tarNode = bluePowerRoot->add_child(Node::node::create());
		tarNode->set_position_3d(vec3(0, 0, 0));
		tarNode->run_action(move_by::create(10.0F, vec3(0, 0, -2)));
		{
			auto const NUM = std::max(gm->getResult().first / 20, 1);
			float const ONE_TIME = 4.8F / NUM;
			redPowerRoot->run_action(repeat_times::create(sequence::create(call_func::create([this, bluePowerRoot, ONE_TIME] {bluePowerRoot->add_child(createPowerTorus(ONE_TIME))->set_color(ColorA(0, 0, 1)); }), delay::create(ONE_TIME)), NUM));
		}
		{
			auto const NUM = std::max(gm->getResult().second / 20, 1);
			float const ONE_TIME = 4.8F / NUM;
			bluePowerRoot->run_action(sequence::create(repeat_times::create(sequence::create(call_func::create([this, bluePowerRoot, ONE_TIME] {bluePowerRoot->add_child(createPowerTorus(ONE_TIME))->set_color(ColorA(0, 0, 1)); }), delay::create(ONE_TIME)), NUM),
				call_func::create([this, power_in, burst, sky]
			{
				power_in->join(burst, [](auto n) { return true; });

				eyeNode->remove_all_actions();
				tarNode->remove_from_parent();
				tarNode = blueCapsuleNode->add_child(Node::node::create());

				redCapsuleNode->run_action(CapsuleNode::length_to::create(1.0F, CANNON_POWER_HALF_LENGTH));

				blueCapsuleNode->run_action(sequence::create(CapsuleNode::length_to::create(1.0F, CANNON_POWER_HALF_LENGTH), call_func::create([this, burst, sky]
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

	burst->onStateIn = [this](auto m)
	{
		
	};

	sky->onStateIn = [this, sky, enemy](auto m)
	{
		auto mat = eyeNode->get_world_matrix_3d();
		eyeNode->remove_from_parent();
		eyeNode = root3d->add_child(Node::node::create());
		eyeNode->set_matrix_3d(mat);

		eyeNode->run_action(sequence::create(ease<ci::EaseOutCubic>::create(move_to::create(1.0F, Game::Field::WORLD_SIZE * vec3(0.5F, 3.0F, 0.5F))), call_func::create([sky, enemy]
		{
			sky->join(enemy, [](auto) { return true; });
		})));
	};

	enemy->onStateIn = [this](auto)
	{
		auto mat = tarNode->get_world_matrix_3d();
		tarNode->remove_from_parent();
		tarNode = root3d->add_child(Node::node::create());
		tarNode->set_matrix_3d(mat);

		auto vec = upNode->get_position_3d();
		eyeNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, vec * 3.0F )));
		tarNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, vec * 3.0F )));

		redCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::length_by::create(1.0F, vec.z * 3.0F)));
		blueCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::length_by::create(1.0F, -vec.z * 3.0F)));
		redCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::radius_by::create(1.0F, vec.z * 0.5F)));
		blueCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::radius_by::create(1.0F, -vec.z * 0.5F)));
	};
	enemy->join(my, [](auto n)
	{
		return n->time > 1.0F;
	});

	my->onStateIn = [this](auto)
	{
		auto vec = upNode->get_position_3d();
		eyeNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, -vec * 3.0F * 2.0F)));
		tarNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, -vec * 3.0F * 2.0F)));

		redCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::length_by::create(1.0F, -vec.z * 3.0F * 2.0F)));
		blueCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::length_by::create(1.0F, vec.z * 3.0F * 2.0F)));
		redCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::radius_by::create(1.0F, -vec.z * 0.5F * 2.0F)));
		blueCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::radius_by::create(1.0F, vec.z * 0.5F * 2.0F)));

		Resource::BGM["result/cannon_power.wav"].fadeout(1.0F, 0.0F);
	};
	my->join(shake, [](auto n)
	{
		return n->time > 1.0F;
	});

	shake->onStateIn = [this](auto)
	{
		auto vec = upNode->get_position_3d();
		eyeNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.2F, vec * 3.0F)));
		tarNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.2F, vec * 3.0F)));

		redCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::length_by::create(1.2F, vec.z * 3.0F)));
		blueCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::length_by::create(1.2F, -vec.z * 3.0F)));
		redCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::radius_by::create(1.2F, vec.z * 0.5F)));
		blueCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::radius_by::create(1.2F, -vec.z * 0.5F)));

		Resource::BGM["result/judge.wav"].play( );

		CAMERA->shakeCamera(0.2F, 1.0F);
	};
	shake->join(judge, [](auto n)
	{
		return n->time > 1.2F;
	});

	judge->onStateIn = [this](auto)
	{
		auto gm = Game::cGameManager::getInstance();

		auto result = gm->getResult();
		float redPoint = result.first;
		float bluePoint = result.second;

		auto sum = redPoint + bluePoint;
		auto redPower = redPoint / sum;
		auto normalizedRedPower = redPower * 2.0F - 1.0F;
		auto bluePower = bluePoint / sum;
		auto normalizedBluePower = bluePower * 2.0F - 1.0F;

		auto vec = upNode->get_position_3d();

		switch (Game::cPlayerManager::getInstance()->getActivePlayerTeamId())
		{
		case Game::Player::Red:
		{
			eyeNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(0.3F, vec3(0, 0, -normalizedRedPower * CANNON_POWER_HALF_LENGTH))));
			tarNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(0.3F, vec3(0, 0, -normalizedRedPower * CANNON_POWER_HALF_LENGTH))));
		}
		case Game::Player::Blue:
		{
			eyeNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(0.3F, vec3(0, 0, -normalizedBluePower * CANNON_POWER_HALF_LENGTH))));
			tarNode->run_action(ease<ci::EaseOutCubic>::create(move_by::create(0.3F, vec3(0, 0, -normalizedBluePower * CANNON_POWER_HALF_LENGTH))));
		}
		default:
			break;
		}

		redCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::length_by::create(0.3F, normalizedRedPower * CANNON_POWER_HALF_LENGTH)));
		blueCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::length_by::create(0.3F, normalizedBluePower * CANNON_POWER_HALF_LENGTH)));
		redCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::radius_by::create(0.3F, normalizedRedPower * 0.5F)));
		blueCapsuleNode->run_action(ease<ci::EaseOutCubic>::create(CapsuleNode::radius_by::create(0.3F, normalizedBluePower * 0.5F)));
	};
	judge->join(score_board, [](auto n)
	{
		return n->time > 1.5F;
	});

	score_board->onStateIn = [this](auto)
	{
		auto move_x = winBoard->get_content_size().x * 2;
		winBoard->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, vec2(move_x, 0))));
		loseBoard->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, vec2(-move_x, 0))));

		if (Game::cGameManager::getInstance()->winTeam() == Game::cPlayerManager::getInstance()->getActivePlayerTeamId())
		{
			Resource::BGM["result/win.wav"].play();
			Resource::BGM["result/win.wav"].setGain(0.5F);
		}
		else
		{
			Resource::BGM["result/lose.wav"].play();
			Resource::BGM["result/lose.wav"].setGain(0.5F);
		}

		bool win = Game::cGameManager::getInstance()->winTeam() == Game::cPlayerManager::getInstance()->getActivePlayerTeamId();
		vec2 winPos = vec2(300, 200) + vec2(-move_x, 0);
		vec2 losePos = rootUI->get_content_size() * vec2(1, 0) + vec2(-300, 200) + vec2(move_x, 0);
		{
			auto p = rootUI->add_child(PlayerNode::create(
				Game::cGameManager::getInstance()->winTeam(),
				Game::cPlayerManager::getInstance()->getActivePlayerTeamId()));
			p->set_position(win ? winPos : losePos);
			p->set_rotation(glm::pi<float>());
			p->set_scale(vec2(100));
			p->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, win ? vec2(move_x, 0) : vec2(-move_x, 0))));
		}
		{
			auto p = rootUI->add_child(PlayerNode::create(
				Game::cGameManager::getInstance()->winTeam(),
				Game::cPlayerManager::getInstance()->getActivePlayerTeamId() == Game::Player::Red ? Game::Player::Blue : Game::Player::Red));
			p->set_position(win ? losePos : winPos);
			p->set_rotation(glm::pi<float>());
			p->set_scale(vec2(100));
			p->run_action(ease<ci::EaseOutCubic>::create(move_by::create(1.0F, win ? vec2(-move_x, 0) : vec2(move_x, 0))));
		}
	};
	score_board->join(win_lose, [this](auto n)
	{
		return n->time > 1.0F;
	});

	win_lose->onStateIn = [this](auto)
	{
		auto id = Game::cPlayerManager::getInstance()->getActivePlayerId();
		if (!(id == 3U || id == 7U))
		{
			auto gm = Game::cGameManager::getInstance();
			auto win = Game::cGameManager::getInstance()->winTeam() == Game::cPlayerManager::getInstance()->getActivePlayerTeamId();

			auto const& image = Resource::IMAGE[win ? "result/win.png" : "result/lose.png"];
			auto logo = rootUI->add_child(Node::Renderer::sprite::create(image));
			logo->set_anchor_point(vec2(0.5F));
			logo->set_color( ColorA(1, 1, 1, 0 ));
			logo->run_action(sequence::create(ease<ci::EaseOutCubic>::create(fade_in::create(1.0F)), call_func::create([this, image]
			{
				auto scaler = rootUI->add_child(Node::Renderer::sprite::create(image));
				scaler->set_anchor_point(vec2(0.5F));
				scaler->run_action(ease<ci::EaseOutCubic>::create(fade_out::create(1.0F)));
				scaler->run_action(ease<ci::EaseOutCubic>::create( scale_by::create(1.0F, vec2( 1.0F ) )));
				scaler->set_position(rootUI->get_content_size() * vec2(0.5F));
			}), delay::create( 4.0F ), ease<ci::EaseInBack>::create(move_by::create(1.0F, vec2(0.0F, -1000.0F)))));
			logo->set_position(rootUI->get_content_size() * vec2(0.5F));
		}
	};

	win_lose->join(bar, [](auto n)
	{
		return n->time > 6.0F;
	});

	bar->onStateIn = [this](auto n)
	{
		auto id = Game::cPlayerManager::getInstance()->getActivePlayerId();
		if (!(id == 3U || id == 7U))
		{
			auto gm = Game::cGameManager::getInstance();
			auto win = Game::cGameManager::getInstance()->winTeam() == Game::cPlayerManager::getInstance()->getActivePlayerTeamId();

			hardptr<Node::node> activePlayerScoreBar;

			auto playerName = Game::cPlayerManager::getInstance()->getActivePlayer()->playerName;
			switch (Game::cPlayerManager::getInstance()->getActivePlayerTeamId())
			{
			case Game::Player::Red:
			{
				activePlayerScoreBar = createScoreBar(playerName,
					gm->getRedTeamAppendGemData()[id],
					gm->getRedTeamKillData()[id],
					gm->getRedTeamDeathData()[id]
				);
				break;
			}
			case Game::Player::Blue:
			{
				activePlayerScoreBar = createScoreBar(playerName,
					gm->getBlueTeamAppendGemData()[id - 4U],
					gm->getBlueTeamKillData()[id - 4U],
					gm->getBlueTeamDeathData()[id - 4U]
				);
				break;
			}
			default:
				break;
			}

			rootUI->add_child(activePlayerScoreBar);
			softptr<Node::node> targetNode;
			if (win)
				targetNode = winBoard->get_child_by_name(playerName);
			else
				targetNode = loseBoard->get_child_by_name(playerName);

			auto mat = targetNode->get_world_matrix_3d(rootUI);
			auto targetPos = vec2(mat[3][0], mat[3][1]);
			activePlayerScoreBar->set_position(targetPos);
			activePlayerScoreBar->run_action(ease<ci::EaseOutCubic>::create(move_to::create(1.0F, vec2(targetPos.x, 350))));
		}
	};

	bar->join(thankyouforplaying, [](auto n)
	{
		return n->time > 5.0F;
	});
	
	thankyouforplaying->onStateIn = [this](auto)
	{
		auto fader = root->add_child(Node::Renderer::rect::create(app::getWindowSize()));
		fader->set_color(ColorA(0, 0, 0, 0));
		fader->set_anchor_point(vec2(0, 0));
		fader->run_action(fade_in::create(2.0F));

		auto l = root->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 120));
		l->set_color(ColorA(1, 1, 1, 0));
		l->set_position(root->get_content_size() * vec2(0.5F));
		l->run_action( fade_in::create( 2.0F ) );
		l->set_text(u8"Thank you for playing.");
		l->set_name("thx");
	};
	thankyouforplaying->join(blackout, [](auto n)
	{
		return n->time > 7.0F;
	});
	thankyouforplaying->onStateOut = [this]
	{
		if (auto l = root->get_child_by_name("thx"))
		{
			l->run_action(Node::Action::fade_out::create(1.0F));
		}
		if (Game::cGameManager::getInstance()->winTeam() == Game::cPlayerManager::getInstance()->getActivePlayerTeamId())
		{
			auto& target = Resource::BGM["result/win.wav"];
			if (target.isPlaying())
			{
				target.fadeout(1.0F, 0.0F);
			}
		}
		else
		{
			auto& target = Resource::BGM["result/lose.wav"];
			if (target.isPlaying())
			{
				target.fadeout(1.0F, 0.0F);
			}
		}
	};

	blackout->join(gototitle, [](auto n)
	{
		return n->time > 1.5F;
	});

	gototitle->onStateIn = [this](auto)
	{
		Scene::cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
	};

	sMac.setEntryNode(power_in);
}
void cResult::shutDown()
{
	Network::cMatchingMemberManager::getInstance()->reset();
	Resource::BGM["result/win.wav"].stop();
	Resource::BGM["result/lose.wav"].stop();
	Resource::BGM["result/cannon_power.wav"].stop();

	ENV->enableKeyButton();
	ENV->enableMouseButton();
	ENV->enablePadButton();
	ENV->enablePadAxis();
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

	if (ENV->pushKey(app::KeyEvent::KEY_RETURN))
	{
		Scene::cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
	}
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
	auto boardRoot = Node::node::create();

	auto boardWithPower = boardRoot->add_child( Node::node::create() );
	boardWithPower->set_content_size(vec2(724 - 142 + 27, 865));
	boardWithPower->set_anchor_point(win ? vec2(0, 0) : vec2(1, 0));
	boardWithPower->set_scale(win ? vec2(1, 1) : vec2(-1, 1));

	if (team == Game::cPlayerManager::getInstance()->getActivePlayerTeamId())
	{
		auto power = boardWithPower->add_child(Node::Renderer::sprite::create(Resource::IMAGE[win ? "result/win_power.png" : "result/lose_power.png"]));
		power->set_anchor_point(vec2(0));
		power->set_position(vec2(142, -373));
	}

	auto board = boardWithPower->add_child( Node::Renderer::sprite::create(Resource::IMAGE[team == Game::Player::Red ? "result/red_board.png" : "result/blue_board.png"]) );
	board->set_anchor_point(vec2(0));
	boardRoot->set_content_size(board->get_content_size());

	auto eastOrWest = boardWithPower->add_child(Node::Renderer::sprite::create(Resource::IMAGE[team == Game::Player::Red ? "result/east.png" : "result/west.png"]));
	eastOrWest->set_anchor_point(vec2(0.5F));
	eastOrWest->set_position(vec2(100, 137));
	eastOrWest->set_scale(win ? vec2(1, 1) : vec2(-1, 1));

	for (int i = 0; i < 3; ++i)
	{
		auto scr = boardRoot->add_child(createScoreBar(playerNameData[i], pointData[i], killData[i], deathData[i]));
		scr->set_position(vec2(27, 220 + i * ( scr->get_content_size().y + 17 )));
	}

	return boardRoot;
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
hardptr<Node::node> cResult::createScoreBar(std::string playerName, int pointData, int killData, int deathData)
{
	auto scr = Node::Renderer::sprite::create(Resource::IMAGE["result/bar.png"]);
	scr->set_anchor_point(vec2(0, 0));
	scr->set_pivot(vec2(0, 0));
	scr->set_name(playerName);

	auto gem = scr->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 30));
	gem->set_text(std::to_string(pointData));
	gem->set_anchor_point(vec2(1, 0.5F));
	gem->set_position(vec2(329, 33));

	auto kill = scr->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 30));
	kill->set_text(std::to_string(killData));
	kill->set_anchor_point(vec2(1, 0.5F));
	kill->set_position(vec2(443, 33));

	auto death = scr->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 30));
	death->set_text(std::to_string(deathData));
	death->set_anchor_point(vec2(1, 0.5F));
	death->set_position(vec2(546, 33));

	auto name = scr->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 30));
	name->set_text(playerName);
	name->set_anchor_point(vec2(0.5F));
	name->set_position(vec2(120, 33 + 3));

	return scr;
}
}
}
