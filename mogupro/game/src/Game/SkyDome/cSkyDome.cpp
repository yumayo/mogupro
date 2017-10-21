#include <Game/SkyDome/cSkyDome.h>
#include <Resource/TextureManager.h>
#include <CameraManager/cCameraManager.h>

void Game::SkyDome::cSkyDome::setup()
{
	//画像のバインド
	TEX->set("sky_dome","SkyDome/Skydome151004y.jpg");

	size = ci::vec3(5000, 5000, 5000);
	
	//画像を貼れる状態にする
	//カリングが適用されなかったのでLambertはなし
	auto shader = ci::gl::ShaderDef().texture();
	mGlsl = ci::gl::getStockShader(shader);
	//頂点数５０のスフィア生成
	auto sphere = ci::geom::Sphere().subdivisions(50);
	mSphere = ci::gl::Batch::create(sphere, mGlsl);

}

void Game::SkyDome::cSkyDome::draw()
{
	
	//画像をバインド
	Resource::TextureManager::getInstance()->get("sky_dome")->bind();
	ci::gl::pushModelView();
	ci::gl::translate(CAMERA->getPos());
	ci::gl::scale(size);
	mSphere->draw();
	ci::gl::popModelView();
	Resource::TextureManager::getInstance()->get("sky_dome")->unbind();


}
