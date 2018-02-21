#pragma once
#include <Collision\cAABBCollider.h>
#include <Game/cLightManager.h>
#include "Resource\cJsonManager.h"
#include <cinder\Json.h>
#include"Resource\cJsonManager.h"
#include <cinder/gl/VboMesh.h>
namespace Game
{
namespace MapObjet
{
	struct PointLight {
		ci::vec3 Pos;
		ci::vec3 color;
		float radius;
		float speed;
		float radiusrate;
		float sinradius = 0.0f;
		bool isupdate;
	};

	struct LineLight {
		ci::vec3 BeginPos;
		ci::vec3 EndPos;
		ci::vec3 color;
		float radius;
		float speed;
		float radiusrate;
		float sinradius = 0.0f;
		bool isupdate;
	};

class cMapObject
{
public:
    cMapObject( );
    ~cMapObject( );
	void setup(const Json::Value & status);
	void update(const float& deltatime );
	void draw();
	void drawAABB();
private:
	void createPointLight(const Json::Value& json);
	void createLineLight(const Json::Value& json);
	void createAABB(const Json::Value& json);
	std::vector<Collision::cAABBCollider> mAabbVector;
	std::vector<Game::Light::PointLightHandle>mPointLights;
	std::vector<PointLight>mPointLightData;
	std::vector<LineLight>mLineLightData;
	std::vector<Game::Light::LineLightHandle>mLineLights;

	// ///// ライトのIDをまとめたデータを用意する
	std::vector<int> pointLightIds;
	std::vector<int> lineLightIds;
	std::vector<int> spotLightIds;

	float a = 0.0f;
	ci::vec3 mPos;
	ci::vec3 mScale;
	ci::vec3 mRotate;
	float mDrawrate;
	int mRotateIndex;
	ci::gl::VboMeshRef mesh;
	std::string texturekey;
	std::string name;
	ci::vec3 jsonToVec3(const Json::Value& json);
};
}
}
