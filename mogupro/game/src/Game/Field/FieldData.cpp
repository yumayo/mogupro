#include <Game/Field/FieldData.h>
namespace Game
{
namespace Field
{
cinder::vec3 const WORLD_SIZE = cinder::vec3( Game::Field::CHUNK_RANGE_X, Game::Field::CHUNK_RANGE_Y, Game::Field::CHUNK_RANGE_Z ) * Game::Field::BLOCK_SIZE * (float)Game::Field::CHUNK_SIZE;
cinder::vec3 const WORLD_COLLISION_SIZE = WORLD_SIZE + cinder::vec3( 0.0F, 100.0F, 0.0F ) - BLOCK_SIZE;
std::vector<cinder::vec3> const RESPAWN_POINT =
{
	{ WORLD_SIZE.x / 2 - 1.5F, WORLD_SIZE.y + 1.0F, 8.0F },
	{ WORLD_SIZE.x / 2		 , WORLD_SIZE.y + 1.0F, 8.0F },
	{ WORLD_SIZE.x / 2 + 1.5F, WORLD_SIZE.y + 1.0F, 8.0F },
	{ WORLD_SIZE.x / 2		 , WORLD_SIZE.y + 7.0F, 8.0F },
	{ WORLD_SIZE.x / 2 - 1.5F, WORLD_SIZE.y + 1.0F, WORLD_SIZE.z - 8.0F },
	{ WORLD_SIZE.x / 2		 , WORLD_SIZE.y + 1.0F, WORLD_SIZE.z - 8.0F },
	{ WORLD_SIZE.x / 2 + 1.5F, WORLD_SIZE.y + 1.0F, WORLD_SIZE.z - 8.0F },
	{ WORLD_SIZE.x / 2		 , WORLD_SIZE.y + 7.0F, WORLD_SIZE.z - 8.0F },
};
std::vector<cinder::quat> const RESPAWN_ROTATION =
{
	glm::angleAxis( 0.0F, cinder::vec3(0, 1, 0) ),
	glm::angleAxis( 0.0F, cinder::vec3(0, 1, 0) ),
	glm::angleAxis( 0.0F, cinder::vec3(0, 1, 0) ),
	glm::angleAxis( 0.0F, cinder::vec3(0, 1, 0) ),
	glm::angleAxis( glm::pi<float>( ), cinder::vec3(0, 1, 0) ),
	glm::angleAxis( glm::pi<float>( ), cinder::vec3(0, 1, 0) ),
	glm::angleAxis( glm::pi<float>( ), cinder::vec3(0, 1, 0) ),
	glm::angleAxis( glm::pi<float>( ), cinder::vec3(0, 1, 0) ),
};
std::vector<cinder::vec3> const CANNON_POINT =
{
	{ WORLD_SIZE.x / 2, WORLD_SIZE.y + 1.0F, 7.0F },
	{ WORLD_SIZE.x / 2, WORLD_SIZE.y + 1.0F, WORLD_SIZE.z - 7.0F },
};
}
}