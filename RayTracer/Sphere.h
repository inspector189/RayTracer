#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "Material.h"
#include "Ray.h"

struct HitRecord
{
    glm::vec3 m_normal;
    glm::vec3 m_point;
    glm::vec3 m_attenuation;
    Ray scattered;
    bool didScattered;
    bool front_face;
    float m_time;
    IMaterial* material;
};

class Sphere
{
public:
    Sphere(const glm::vec3& center, float radius, const glm::vec3& albedo, IMaterial::MaterialType type, float fuzz = 0.0f);
    ~Sphere();
    Sphere(const Sphere& other);
    Sphere& operator=(const Sphere& other);
    Sphere(Sphere&& other) noexcept;
    Sphere& operator=(Sphere&& other) noexcept;
    bool hit(const Ray& ray, float minTime, float maxTime, HitRecord* hitRecord) const;

private:
    void cleanup();

    glm::vec3 m_center;
    float m_radius;
    IMaterial* m_material;
};