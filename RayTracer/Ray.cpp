#include "Ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
{
	m_origin = origin;
	m_direction = direction;
}
Ray::Ray()
{
	m_origin = glm::vec3(0,0,0);
	m_direction = glm::vec3(0, 0, 0);
}
glm::vec3 Ray::at(float t) const
{
	return m_origin + m_direction * t;
}

glm::vec3 Ray::getDirection() const
{
	return m_direction;
}

glm::vec3 Ray::getOrigin() const
{
	return m_origin;
}
