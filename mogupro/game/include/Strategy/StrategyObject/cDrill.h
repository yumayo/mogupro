#pragma once
#include"Strategy\StrategyObject\cStrategyObjectBase.h"
namespace Strategy
{
namespace StrategyObject
{
class cDrill: public cStrategyObjectBase
{
public:
	cDrill(const ci::vec3 _pos,const ci::vec3 _scale);
    ~cDrill( );
	void draw() override;
	void update(const float& delta_time) override;
	void setup() override;
	bool DeleteThis() override;
private:
	struct DrillSlope
	{
		ci::vec3 pos;
		ci::vec3 scale;
		float rotate;
	};
	void drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color);
	std::vector<DrillSlope>drillslopes;
	void createDrills();
	void move();
	float drillspeed = 0.2f;
	float y_rotate = 0.0f;
	ci::vec3 beginpos;
	ci::vec3 machinescale;
	ci::vec3 slope;
	float drillslope = 0.0f;
};
}
}
