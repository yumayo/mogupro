#include <Game/MapObjet/cMapObject.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>
#include"Game\Field\FieldData.h"
#include"Game\cStrategyManager.h"
#include"cinder\Rand.h"
#include <Game/cShaderManager.h>	
namespace Game
{
namespace MapObjet
{
cMapObject::cMapObject( )
{

}
cMapObject::~cMapObject( )
{
	for (int i = 0; i < mAabbVector.size(); i++) {
		mAabbVector[i].removeWorld();
	}
}
void cMapObject::setup(const Json::Value & status)
{
	mPos = jsonToVec3(status["pos"]) + ci::vec3(0, Game::Field::WORLD_SIZE.y + Game::Field::BLOCK_SIZE, 0);
	name = status["name"].asString();
	texturekey = Resource::JSON[name + ".json"]["asset"].asString();
	mDrawrate = Resource::JSON[name + ".json"]["drawrate"].asFloat();
	mScale = jsonToVec3(status["scale"])*mDrawrate;
	mRotateIndex = status["rotatey"].asInt();
	mRotate = ci::vec3(0, float(mRotateIndex)*M_PI / 2.f, 0);
	

	mesh = Resource::cObjectManager::getInstance()->findObject("MapObject/" + texturekey + ".obj");

	TEX->set(texturekey, "OBJ/MapObject/" + texturekey + ".png");

	createPointLight(Resource::JSON[name+ ".json"]["PointLight"]);
	createLineLight(Resource::JSON[name + ".json"]["LineLight"]);

	createAABB(Resource::JSON[name + ".json"]["AABB"]);

	// /////////ライトのデータを詰め込む。
	for (auto& p_light : mPointLights) pointLightIds.emplace_back(p_light->getId());
	for (auto& l_light : mLineLights) lineLightIds.emplace_back(l_light->getId());
}


void cMapObject::update(const float & deltatime)
{
	for (int i = 0; i < mPointLights.size(); i++) {
		if (!mPointLightData[i].isupdate)continue;
		mPointLights[i]->reAttachRadius(mPointLightData[i].radius*mPointLightData[i].radiusrate + mPointLightData[i].radius*(1.0f- mPointLightData[i].radiusrate)*sin(mPointLightData[i].sinradius));
		mPointLightData[i].sinradius += mPointLightData[i].speed*deltatime;
	}
	for (int i = 0; i < mLineLights.size(); i++) {
		if (!mLineLightData[i].isupdate)continue;
		mLineLights[i]->reAttachRadius(mLineLightData[i].radius*mLineLightData[i].radiusrate + mLineLightData[i].radius*(1.0f- mLineLightData[i].radiusrate)*sin(mLineLightData[i].sinradius));
		mLineLightData[i].sinradius += mLineLightData[i].speed*deltatime;
	}
}

void cMapObject::draw()
{
	// ////////////まとめておいたIDを使ってライトを反映させる。
	cShaderManager::getInstance()->uniformUpdate(pointLightIds, lineLightIds, spotLightIds);
	{
		ci::gl::ScopedTextureBind tex(TEX->get(texturekey));

		ci::gl::pushModelView();
		ci::gl::translate(mPos);
		ci::gl::rotate(mRotate.y, ci::vec3(0, 1, 0));
		ci::gl::scale(mScale);
		ci::gl::color(ci::ColorA(1, 1, 1, 1));
		ci::gl::draw(mesh);
		ci::gl::popModelView();
	}

}
void cMapObject::drawAABB()
{
	for (int i = 0; i < mAabbVector.size(); i++) {
		float h = ((float(i)) / float(mAabbVector.size()));
		ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.0f));
		STRM->drawCube(mAabbVector[i].getPosition(), mAabbVector[i].getSize(), ci::vec3(0), ci::ColorA(hsv.r, hsv.g, hsv.b, 1));
	}
	
}

void cMapObject::createPointLight(const Json::Value & json)
{
	for (int i = 0; i < json.size(); i++) {
		PointLight pointlight;
		pointlight.Pos = mPos + mScale * jsonToVec3(json[i]["pos"])/mDrawrate;
		float length = glm::distance(ci::vec2(mPos.x, mPos.z), ci::vec2(pointlight.Pos.x, pointlight.Pos.z));
		float angle = std::atan2f(pointlight.Pos.z - mPos.z, pointlight.Pos.x - mPos.x);
		pointlight.Pos = ci::vec3(mPos.x + length*cos(angle-mRotate.y), pointlight.Pos.y, mPos.z + length*sin(angle - mRotate.y));
		pointlight.radius = json[i]["radius"].asFloat()*mScale.x / mDrawrate;
		pointlight.radiusrate = json[i]["radiusrate"].asFloat();
		pointlight.color = jsonToVec3(json[i]["color"]);
		pointlight.isupdate = json[i]["isupdate"].asBool();
		pointlight.sinradius = ci::randFloat(2.f*M_PI);
		pointlight.speed = json[i]["speed"].asFloat();
		mPointLightData.push_back(pointlight);
	}
	for (int i = 0; i < mPointLightData.size(); i++) {
		mPointLights.push_back(cLightManager::getInstance()->addPointLight(
			mPointLightData[i].Pos,
			mPointLightData[i].color, 
			mPointLightData[i].radius));
	}
}

void cMapObject::createLineLight(const Json::Value & json)
{
	for (int i = 0; i < json.size(); i++) {
		LineLight linelight;
		linelight.BeginPos = mPos + mScale * jsonToVec3(json[i]["beginpos"]) / mDrawrate;

		float beginlength = glm::distance(ci::vec2(mPos.x, mPos.z), ci::vec2(linelight.BeginPos.x, linelight.BeginPos.z));
		float beginangle = std::atan2f(linelight.BeginPos.z - mPos.z, linelight.BeginPos.x - mPos.x);
		linelight.BeginPos = ci::vec3(mPos.x + beginlength*cos(beginangle - mRotate.y), linelight.BeginPos.y, mPos.z + beginlength*sin(beginangle - mRotate.y));

		linelight.EndPos = mPos + mScale * jsonToVec3(json[i]["endpos"]) / mDrawrate;

		float endlength = glm::distance(ci::vec2(mPos.x, mPos.z), ci::vec2(linelight.EndPos.x, linelight.EndPos.z));
		float endangle = std::atan2f(linelight.EndPos.z - mPos.z, linelight.EndPos.x - mPos.x);
		linelight.EndPos = ci::vec3(mPos.x + endlength*cos(endangle - mRotate.y), linelight.EndPos.y, mPos.z + endlength*sin(endangle - mRotate.y));


		linelight.radius = json[i]["radius"].asFloat()*mScale.x / mDrawrate;
		linelight.radiusrate = json[i]["radiusrate"].asFloat();
		linelight.color = jsonToVec3(json[i]["color"]);
		linelight.isupdate = json[i]["isupdate"].asBool();
		linelight.sinradius = ci::randFloat(2.f*M_PI);
		linelight.speed = json[i]["speed"].asFloat();
		mLineLightData.push_back(linelight);
	}
	for (int i = 0; i < mLineLightData.size(); i++) {
		mLineLights.push_back(cLightManager::getInstance()->addLineLight(
			mLineLightData[i].BeginPos, mLineLightData[i].EndPos,
			mLineLightData[i].color,
			mLineLightData[i].radius));
	}
}

void cMapObject::createAABB(const Json::Value & json)
{
	for (int i = 0; i < json.size(); i++) {
		ci::vec3 trancepos = mPos + mScale * jsonToVec3(json[i]["center"]) / mDrawrate;
		ci::vec3 scalerate = mScale * jsonToVec3(json[i]["scalerate"]) / mDrawrate;
		if (mRotateIndex % 2 == 0) {
			mAabbVector.push_back(Collision::cAABBCollider(trancepos, scalerate));
		}
		else {
			mAabbVector.push_back(Collision::cAABBCollider(trancepos, ci::vec3(scalerate.z, scalerate.y, scalerate.x)));
		}
		if (json[i].isMember("camera")) {
			mAabbVector[i].setLayer(json[i]["camera"].asInt());
		}
		mAabbVector[i].addWorld();
	}
}


ci::vec3 cMapObject::jsonToVec3(const Json::Value & json)
{
	ci::vec3 Vec3;
	Vec3.x = json[0].asFloat();
	Vec3.y = json[1].asFloat();
	Vec3.z = json[2].asFloat();

	return Vec3;
}

}
}
