#include <Utility/TriMeshAnimation.h>
#include <cinder/Rand.h>
#include <cinder/TriMesh.h>
namespace Utility
{
	void TriMeshAnimation::make(ci::vec2 windowSize, ci::vec2 cutSize)
	{
		this->windowSize = windowSize;
		indexCount = 0;
		ci::vec2 size(windowSize / cutSize);
		ci::vec3 gravityPoint = ci::vec3(size.x / 3.0f, size.y / 3.0f, 0);

		//!@ LookMe : PosA,B,C‚É‚µ‚Ä‚¢‚é‚Ì‚ÍdS‚ğ’†S‚É‰ñ“]‚³‚¹‚½‚¢‚©‚ç
		for (int y = 0; y < cutSize.y; ++y)
		{
			for (int x = 0; x < cutSize.x; ++x)
			{
				centers.push_back(gravityPoint + ci::vec3(size.x * x, size.y * y, 0.0f));
				posAs.push_back(-gravityPoint);
				posBs.push_back(ci::vec3(size.x, 0, 0.0f) - gravityPoint);
				posCs.push_back(ci::vec3(0, size.y, 0.0f) - gravityPoint);

				centers.push_back(ci::vec3(size.x, size.y, 0.0f) - gravityPoint + ci::vec3(size.x * x, size.y * y, 0.0f));
				posAs.push_back(ci::vec3(-size.x, 0, 0.0f) + gravityPoint);
				posBs.push_back(ci::vec3(0, -size.y, 0.0f) + gravityPoint);
				posCs.push_back(gravityPoint);

				texCoords.push_back(ci::vec2(0.0f, 0.0f) + ci::vec2((1.0f / cutSize.x) * x, (1.0f / cutSize.y) * y));
				texCoords.push_back(ci::vec2((1.0f / cutSize.x), 0.0f) + ci::vec2((1.0f / cutSize.x) * x, (1.0f / cutSize.y)* y));
				texCoords.push_back(ci::vec2(0.0f, (1.0f / cutSize.y)) + ci::vec2((1.0f / cutSize.x) * x, (1.0f / cutSize.y)* y));
				texCoords.push_back(ci::vec2(0.0f, (1.0f / cutSize.y)) + ci::vec2((1.0f / cutSize.x) * x, (1.0f / cutSize.y)* y));
				texCoords.push_back(ci::vec2((1.0f / cutSize.x), 0.0f) + ci::vec2((1.0f / cutSize.x) * x, (1.0f / cutSize.y)* y));
				texCoords.push_back(ci::vec2((1.0f / cutSize.x), (1.0f / cutSize.y))
					+ ci::vec2((1.0f / cutSize.x) * x, (1.0f / cutSize.y)* y));

				for (int i = 0; i < 6; ++i)
				{
					indices.push_back(indexCount);
					++indexCount;
				}
			}
		}

		for (int i = 0; i < (int)indices.size() / 3; ++i)
		{
			vertices.push_back(centers[i] + posAs[i]);
			vertices.push_back(centers[i] + posBs[i]);
			vertices.push_back(centers[i] + posCs[i]);
		}

		for (int i = 0; i < (int)indices.size() / 3; ++i)
		{
			vectors.push_back(ci::vec3(ci::randFloat(-1.5f, 1.5f), ci::randFloat(-1.5f, 1.5f), ci::randFloat(1.5f, - 1.5f)));
			rotate.push_back(ci::vec2(ci::randFloat(-0.5f, 0.5f), ci::randFloat(-0.5f, 0.5f)));
		}
	}

	void TriMeshAnimation::update()
	{
		for (int i = 0; i < (int)indices.size() / 3; ++i)
		{
			posAs[i] = glm::rotateX(posAs[i], rotate[i].x);
			posBs[i] = glm::rotateX(posBs[i], rotate[i].x);
			posCs[i] = glm::rotateX(posCs[i], rotate[i].x);

			posAs[i] = glm::rotateZ(posAs[i], rotate[i].y);
			posBs[i] = glm::rotateZ(posBs[i], rotate[i].y);
			posCs[i] = glm::rotateZ(posCs[i], rotate[i].y);

			centers[i] += vectors[i];
		}

		for (int i = 0; i < (int)indices.size() / 3; ++i)
		{
			vertices[0 + i * 3] = centers[i] + posAs[i];
			vertices[1 + i * 3] = centers[i] + posBs[i];
			vertices[2 + i * 3] = centers[i] + posCs[i];
		}
	}

	void TriMeshAnimation::draw()
	{
		ci::TriMeshRef mesh = ci::TriMesh::create(ci::TriMesh::Format().positions(3).texCoords(2));

		ci::gl::pushModelView();
		mesh->appendPositions(vertices.data(), (int)vertices.size());
		mesh->appendTexCoords0(texCoords.data(), (int)texCoords.size());
		mesh->appendIndices(indices.data(), (int)indices.size());
		ci::gl::translate(ci::vec2(-windowSize.x / 2, -windowSize.y / 2));
		ci::gl::draw(*mesh);
		ci::gl::popModelView();
	}

}
