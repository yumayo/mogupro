#include <Game/MapObjet/cMapObject.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>
#include"Game\Field\FieldData.h"
namespace Game
{
namespace MapObjet
{
cMapObject::cMapObject( )
{

}
cMapObject::~cMapObject( )
{
	/*for (int i = 0; i < mAabbVector.size(); i++) {
		mAabbVector[i].removeWorld();
	}*/
}
void cMapObject::setup()
{
	//texturekey = "lamp";
	//mesh = Resource::cObjectManager::getInstance()->findObject("MapObject/" + texturekey + ".obj");
	//TEX->set(texturekey, "OBJ/MapObject/" + texturekey + ".png");
	/*mScale = ci::vec3(0.1, 0.1, 0.1);

	mPos = ci::vec3(5, Game::Field::WORLD_SIZE.y + 1, 5);*/
}

void cMapObject::update(const float & deltatime)
{

}

void cMapObject::draw()
{
	//ci::gl::ScopedTextureBind tex(TEX->get(texturekey));

	//ci::gl::pushModelView();
	//ci::gl::translate(mPos);
	//ci::gl::scale(mScale);
	//ci::gl::draw(mesh);
	//ci::gl::popModelView();
}
void cMapObject::createPointLight(const ci::JsonTree & jsontree)
{

}
void cMapObject::createLineLight(const ci::JsonTree & jsontree)
{

}
void cMapObject::createAABB(const ci::JsonTree & jsontree)
{

}
//ci::vec3 cMapObject::jsonToVec3(const ci::JsonTree & jsontree, const std::string key)
//{
//	if (key == "") {
//		ci::vec3 Vec3;
//		Vec3.x = jsontree.getValueAtIndex<float>(0);
//		Vec3.y = jsontree.getValueAtIndex<float>(1);
//		Vec3.z = jsontree.getValueAtIndex<float>(2);
//		return Vec3;
//	}
//	else {
//		auto vec = jsontree[key];
//		ci::vec3 Vec3;
//		Vec3.x = vec.getValueAtIndex<float>(0);
//		Vec3.y = vec.getValueAtIndex<float>(1);
//		Vec3.z = vec.getValueAtIndex<float>(2);
//		return Vec3;
//	}
//}
}
}
