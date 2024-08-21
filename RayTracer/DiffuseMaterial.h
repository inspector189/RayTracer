#pragma once
#include <glm.hpp>

class DiffuseMaterial
{
public:
	DiffuseMaterial(const glm::vec3& albedo);
private:
	glm::vec3 m_albedo;
};

