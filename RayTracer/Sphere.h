#pragma once
#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/norm.hpp>
#include "Ray.h"
#include "DiffuseMaterial.h"

struct HitRecord
{
	glm::vec3 m_normal;
	glm::vec3 m_point;
	glm::vec3 m_attenuation;
	float m_time;
};

class Sphere
{
public:
	Sphere(const glm::vec3& center, float radius, const glm::vec3& albedo);

	bool hit(const Ray& ray, float minTime, float maxTime, HitRecord* hitRecord) const;
private:
	glm::vec3 m_center;
	float m_radius;
	DiffuseMaterial m_diffuseMaterial;
};
