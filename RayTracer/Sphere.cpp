#include "Sphere.h"

Sphere::Sphere(const glm::vec3& center, float radius)
{
	m_center = center;
	m_radius = radius;
}
bool Sphere::hit(const Ray& ray, float minTime, float maxTime, HitRecord* hitRecord) const
{
    glm::vec3 oc = m_center - ray.getOrigin();
    float a = glm::length2(ray.getDirection());
    float h = glm::dot(ray.getDirection(), oc);
    float c = glm::length2(oc) - m_radius * m_radius;
    float discriminant = h * h - a * c;
    if (discriminant < 0)
    {
        return false;
    }
    float sqrtD = std::sqrt(discriminant);

    float root = (h - sqrtD) / a;
    if (root <= minTime || root >= maxTime)
    {
        root = (h + sqrtD) / a;
        if (root <= minTime || root >= maxTime)
        {
            return false;
        }
    }
    hitRecord->m_time = root;
    hitRecord->m_point = ray.at(root);
    hitRecord->m_normal = (hitRecord->m_point - m_center)/m_radius;
    return true;
}
