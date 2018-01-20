#pragma once
#include <Scene/cSceneBase.h>
namespace Scene
{
namespace Member
{
class cWeaponSelect final : cSceneBase
{
public:
    cWeaponSelect( ) = default;
    ~cWeaponSelect( ) = default;
public:
	void setup( ) override;
	void shutDown( ) override;
	void update( float t ) override;
	void draw( ) override;
	void draw2D( ) override;
	void resize( ) override;
	std::string const& getName( ) { return typeid( *this ).name( ); }
};
}
}
