#pragma once
#include <Collision\cAABBCollider.h>
#include <Game/cLightManager.h>
#include "Resource\cJsonManager.h"
#include <cinder\Json.h>
//#include <cinder/gl/VboMesh.h>
namespace Game
{
namespace MapObjet
{
	struct PointLight {
		ci::vec3 Pos;
		ci::vec3 color;
		float radius;
		bool isupdate;
	};

	struct LineLight {
		ci::vec3 BeginPos;
		ci::vec3 EndPos;
		ci::vec3 color;
		float radius;
		bool isupdate;
	};

class cMapObject
{
public:
    cMapObject( );
    ~cMapObject( );
	void setup();
	void update(const float& deltatime );
	void draw();
private:
	void createPointLight(const ci::JsonTree& jsontree);
	void createLineLight(const ci::JsonTree& jsontree);
	void createAABB(const ci::JsonTree& jsontree);
	//std::vector<Collision::cAABBCollider> mAabbVector;
	//std::vector<Game::Light::PointLightHandle>mPointLights;
	//std::vector<Game::Light::LineLightHandle>mLineLights;
	//ci::vec3 mPos;
	//ci::vec3 mScale;
	//ci::gl::VboMeshRef mesh;
	//std::string texturekey;
	//ci::vec3 jsonToVec3(const ci::JsonTree & jsontree, const std::string key);
};
}
}
