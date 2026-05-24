#include "Sphere.h"

Sphere::Sphere(const glm::vec3& center, float radius, const glm::vec3& albedo, IMaterial::MaterialType type, float fuzzOrIOR)
    : m_center(center), m_radius(radius), m_material(nullptr)
{
    if (type == IMaterial::MaterialType::diffuse)
        m_material = new DiffuseMaterial(albedo);
    else if (type == IMaterial::MaterialType::metal)
        m_material = new MetalMaterial(albedo, fuzzOrIOR);
    else if (type == IMaterial::MaterialType::dielectric)
        m_material = new DielectricMaterial(fuzzOrIOR);
}

Sphere::~Sphere()
{
    cleanup();
}

Sphere::Sphere(const Sphere& other)
    : m_center(other.m_center), m_radius(other.m_radius), m_material(nullptr)
{
    if (other.m_material)
    {
        m_material = other.m_material->clone();
    }
}

Sphere& Sphere::operator=(const Sphere& other)
{
    if (this != &other)
    {
        cleanup();
        m_center = other.m_center;
        m_radius = other.m_radius;
        if (other.m_material)
        {
            m_material = other.m_material->clone();
        }
    }
    return *this;
}

Sphere::Sphere(Sphere&& other) noexcept
    : m_center(other.m_center), m_radius(other.m_radius), m_material(other.m_material)
{
    other.m_material = nullptr;
}

Sphere& Sphere::operator=(Sphere&& other) noexcept
{
    if (this != &other)
    {
        cleanup();
        m_center = other.m_center;
        m_radius = other.m_radius;
        m_material = other.m_material;
        other.m_material = nullptr;
    }
    return *this;
}

bool Sphere::hit(const Ray& ray, float minTime, float maxTime, HitRecord* hitRecord) const
{
    glm::vec3 oc = ray.getOrigin() - m_center;
    float a = glm::length2(ray.getDirection());
    float h = glm::dot(ray.getDirection(), oc);
    float c = glm::length2(oc) - m_radius * m_radius;
    float discriminant = h * h - a * c;

    if (discriminant < 0)
        return false;

    float sqrtD = std::sqrt(discriminant);
    float root = (-h - sqrtD) / a;
    if (root < minTime || root > maxTime)
    {
        root = (-h + sqrtD) / a;
        if (root < minTime || root > maxTime)
            return false;
    }

    hitRecord->m_time = root;
    hitRecord->m_point = ray.at(root);
    glm::vec3 outward_normal = glm::normalize(hitRecord->m_point - m_center);

    hitRecord->front_face = glm::dot(ray.getDirection(), outward_normal) < 0;
    if (!hitRecord->front_face)
        hitRecord->m_normal = -outward_normal;
    else
        hitRecord->m_normal = outward_normal;

   // hitRecord->m_normal = outward_normal;
    hitRecord->material = m_material; 
    return true;
}

void Sphere::cleanup()
{
    delete m_material;
    m_material = nullptr;
}