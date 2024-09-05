#pragma once
#include <glm.hpp>

class DiffuseMaterial
{
public:
	DiffuseMaterial(const glm::vec3& albedo);
	glm::vec3 scatter() const;
private:
	glm::vec3 m_albedo;
};

