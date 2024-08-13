#pragma once
#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/norm.hpp>
#include "Ray.h"

struct HitRecord
{
	glm::vec3 m_normal;
	glm::vec3 m_point;
	float m_time;
};

class Sphere
{
public:
	Sphere(const glm::vec3& center, float radius);

	bool hit(const Ray& ray, float minTime, float maxTime, HitRecord* hitRecord) const;
private:
	glm::vec3 m_center;
	float m_radius;
};
