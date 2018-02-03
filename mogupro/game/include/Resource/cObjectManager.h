#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <cinder/gl/VboMesh.h>
#include <cinder/TriMesh.h>
#include <vector>
#include <string>
namespace Resource
{
class cObjectManager : public Utility::cSingletonAble<cObjectManager>
{
public:
    cObjectManager( );
    // 例: Assets/OBJ/player.obj なら
    // ＞: player.obj と入力する。
    cinder::gl::VboMeshRef findObject( std::string const& underAssetsUnderSEUnderPath );
	cinder::TriMeshRef findMesh( std::string const& underAssetsUnderSEUnderPath );
	void loadOne( );
	bool isFinished( );
	int maxNum( );
	int currentNum( );
private:
	struct Mesh
	{
		cinder::gl::VboMeshRef vbo;
		cinder::TriMeshRef mesh;
	};
    std::map<std::string, Mesh> mObjects;
	int mCurrentLoadIndex = 0;
	std::vector<std::string> mFilePaths;
};
struct ObjectSuppoter
{
	class ObjectNotFound : public std::runtime_error
	{
	public:
		ObjectNotFound( ) : std::runtime_error( "objファイルが見つかりません。" )
		{
		}
	};
	cinder::gl::VboMeshRef operator[]( std::string const& underAssetsUnderOBJUnderPath ) const
	{
		try
		{
			return cObjectManager::getInstance( )->findObject( underAssetsUnderOBJUnderPath );
		}
		catch ( ... )
		{
			throw ObjectNotFound( );
		}
	}
};
extern ObjectSuppoter const OBJ;
struct MeshSuppoter
{
	class MeshNotFound : public std::runtime_error
	{
	public:
		MeshNotFound( ) : std::runtime_error( "objファイルが見つかりません。" )
		{
		}
	};
	cinder::TriMeshRef operator[]( std::string const& underAssetsUnderOBJUnderPath ) const
	{
		try
		{
			return cObjectManager::getInstance( )->findMesh( underAssetsUnderOBJUnderPath );
		}
		catch ( ... )
		{
			throw MeshNotFound( );
		}
	}
};
extern MeshSuppoter const MESH;
}
