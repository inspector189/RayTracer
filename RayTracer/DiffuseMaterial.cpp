#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial(const glm::vec3& albedo)
{
	m_albedo = albedo;
}

glm::vec3 DiffuseMaterial::scatter() const
{
	return m_albedo;
}