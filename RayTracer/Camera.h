#pragma once
#include <glm.hpp>
#include "Ray.h"

class Camera
{
public:
	Camera(const glm::vec3& cameraPosition, const glm::vec3& lookAt, const glm::vec3& up, float fov, float aspectRatio, float aperture, float focusDistance);

	Ray generateRay(float s, float t) const;
	
private:	
	glm::vec3 m_cameraPosition;
	glm::vec3 m_lowerLeft;
	glm::vec3 m_horizontal;
	glm::vec3 m_vertical;
	glm::vec3 m_w;
	glm::vec3 m_u;
	glm::vec3 m_v;
	float m_lensRadius;

	glm::vec3 RandomInUnitDisk() const;
};

