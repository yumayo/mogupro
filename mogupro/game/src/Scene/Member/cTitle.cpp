#include <Scene/Member/cTitle.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Scene/Member/cGameMain.h>
#include <Scene/Member/cMatching.h>
#include <Scene/Member/cMatchingServer.h>
#include <Scene/cSceneManager.h>
#include <Network.hpp>
#include <Game/cGameManager.h>
#include <Network/cMatchingMemberManager.h>
#include <Resource/cImageManager.h>
#include <Scene/Member/Tutorial.h>
#include <Game/Player/cPlayer.h>
#include <Sound/Wav.h>
#include <Scene/Member/cModeSelect.h>
using namespace cinder;
using namespace Node::Action;
namespace Scene
{
namespace Member
{
cTitle::cTitle( )
{

}
cTitle::~cTitle( )
{

}
void cTitle::setup( )
{
	ENV->setMouseControl(false);
	ENV->enableKeyButton();
	ENV->enableMouseButton();
	ENV->enablePadAxis();
	ENV->enablePadButton();

	Network::cUDPClientManager::removeInstance();
	Network::cUDPServerManager::removeInstance();

	STATE_GENERATE(sMac, init);
	STATE_GENERATE(sMac, fadein);
	STATE_GENERATE(sMac, idle);
	STATE_GENERATE(sMac, fadeout);
	STATE_GENERATE(sMac, nextScene);

	init->join(fadein, [](auto n)
	{
		return true;
	});
	init->onStateIn = [this](auto)
	{
		root = Node::node::create();
		root->set_content_size(cinder::app::getWindowSize());
		root->set_scale(cinder::vec2(1, -1));
		root->set_position(root->get_content_size() * cinder::vec2(-0.5F, 0.5F));

		auto backGround = root->add_child(Node::Renderer::sprite::create(Resource::IMAGE["title/background.png"]));
		backGround->set_anchor_point( vec2(0) );

		auto contetnts = root->add_child(Node::node::create());
		contetnts->set_content_size(app::getWindowSize() - ivec2(100, 100));
		contetnts->set_position(ivec2(100, 100) / 2);

		auto logo = contetnts->add_child(Node::Renderer::sprite::create(Resource::IMAGE["title/logo.png"]));
		logo->set_anchor_point(vec2(0.0F));
		logo->set_position(vec2(0.0F));

		auto pushanybutton = contetnts->add_child(Node::Renderer::sprite::create(Resource::IMAGE["title/pushanybutton.png"]));
		pushanybutton->set_anchor_point(vec2(1, 1));
		pushanybutton->set_position(contetnts->get_content_size());

		auto version = contetnts->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 32.0F));
		version->set_anchor_point(vec2(0, 1));
		version->set_position(contetnts->get_content_size() * vec2(0, 1));
		version->set_text("ver0.4.0");

		fader = root->add_child( Node::Renderer::rect::create( root->get_content_size( ) ) );
		fader->set_color( ColorA( 0, 0, 0, 1 ) );
		fader->set_anchor_point(vec2(0));
		fader->run_action( Node::Action::fade_out::create(1.0F) );

		auto bgm = Sound::Wav(cinder::app::getAssetDirectories().front().string() + "/BGM/title/background.wav");
		introloopBGM.create(bgm.data(), bgm.size(), 0.0F, 60.0F + 18.59638F);
		introloopBGM.gain(0.0F);
		introloopBGM.fadein(1.0F, 0.3F);

		CAMERA->setup();
	};
	fadein->join(idle, [](auto n)
	{
		return n->time > 1.0F;
	});
	idle->join(fadeout, [fadeout](auto n)
	{
		return fadeout->onStateOut != nullptr;
	});
	fadeout->onStateIn = [this](auto)
	{
		introloopBGM.fadeout(1.0F, 0.0F);
		fader->run_action(Node::Action::fade_in::create( 1.0F ));
	};
	fadeout->join(nextScene, [](auto n)
	{
		return n->time > 1.2F;
	});

	this->fadeout = fadeout;

	sMac.setEntryNode(init);
}
void cTitle::shutDown( )
{
	introloopBGM.stop();
}
void cTitle::resize( )
{
}
void cTitle::update( float deltaTime )
{
	sMac.update(deltaTime);
	introloopBGM.update( deltaTime );
	root->entry_update( deltaTime );

	if (!fadeout->onStateOut)
	{
		if (ENV->pushKey(cinder::app::KeyEvent::KEY_F1))
		{
			fadeout->onStateOut = [] {cSceneManager::getInstance()->shift<Scene::Member::cMatching>(); };
		}
		else if (ENV->pushKey(cinder::app::KeyEvent::KEY_F2))
		{
			fadeout->onStateOut = [] {cSceneManager::getInstance()->shift<Scene::Member::cMatchingServer>(); };
		}
		else if (Utility::cInputAll::getInstance()->pushKey(cinder::app::KeyEvent::KEY_F3))
		{
			fadeout->onStateOut = [] {
				Network::cUDPClientManager::getInstance()->open();
				Network::cUDPServerManager::getInstance()->open();
				Network::cUDPClientManager::getInstance()->connectOfflineServer();
				Game::cGameManager::getInstance()->setTime(0.0F);
				Network::cMatchingMemberManager::getInstance()->mPlayerID = 3U;
				cSceneManager::getInstance()->shift<Scene::Member::cGameMain>();
			};
		}
		else if (ENV->pushKey(cinder::app::KeyEvent::KEY_F5))
		{
			fadeout->onStateOut = [] {
				Network::cUDPClientManager::getInstance()->open();
				Network::cUDPServerManager::getInstance()->open();
				Network::cUDPClientManager::getInstance()->connectOfflineServer();
				Game::cGameManager::getInstance()->setTime(0.0F);
				cSceneManager::getInstance()->shift<Scene::Member::cTutorial>();
			};
		}
		else if (ENV->pushKey(cinder::app::KeyEvent::KEY_F10))
		{
			fadeout->onStateOut = [] {
				Network::cUDPClientManager::getInstance()->open();
				Network::cUDPServerManager::getInstance()->open();
				Network::cUDPClientManager::getInstance()->connectOfflineServer();
				Game::cGameManager::getInstance()->setTime(0.0F);
				Network::cMatchingMemberManager::getInstance()->mPlayerID = 4U;
				Network::cMatchingMemberManager::getInstance()->mPlayerTeamNum = Game::Player::Team::Blue;
				cSceneManager::getInstance()->shift<Scene::Member::cGameMain>();
			};
		}
		else if (ENV->pushKey(cinder::app::KeyEvent::KEY_0)) {
			fadeout->onStateOut = [] {
				Network::cUDPClientManager::getInstance()->open();
				Network::cUDPServerManager::getInstance()->open();
				Network::cUDPClientManager::getInstance()->connectOfflineServer();
				Game::cGameManager::getInstance()->setTime(0.0F);
				cSceneManager::getInstance()->shift<Scene::Member::cGameMain>();
			};
		}
		else if (ENV->pushKey())
		{
			fadeout->onStateOut = [] {cSceneManager::getInstance()->shift<Scene::Member::cModeSelect>(); };
		}
	}
}
void cTitle::draw( )
{
}
void cTitle::draw2D( )
{
	root->entry_render( cinder::mat4( ) );
}
}
}
