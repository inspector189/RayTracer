#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "Math.h"

struct HitRecord;

class IMaterial
{
public:
    enum class MaterialType
    {
        diffuse,
        metal,
        dielectric
    };

    virtual ~IMaterial() = default;
    virtual glm::vec3 scatter(const Ray& ray, HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const = 0;
    virtual IMaterial* clone() const = 0; 
};

class MetalMaterial : public IMaterial
{
public:
    MetalMaterial(const glm::vec3& albedo, double fuzz);
    glm::vec3 scatter(const Ray& ray, HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override;
    IMaterial* clone() const override { return new MetalMaterial(*this); }

private:
    glm::vec3 m_albedo;
    glm::vec3 reflect(const glm::vec3 v, const glm::vec3 n) const {
        return v - 2.0f * glm::dot(v, n) * n;
    }

    float m_fuzz;
};

class DiffuseMaterial : public IMaterial
{
public:
    DiffuseMaterial(const glm::vec3& albedo);
    glm::vec3 scatter(const Ray& ray, HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override;
    IMaterial* clone() const override { return new DiffuseMaterial(*this); }

private:
    glm::vec3 m_albedo;
};

class DielectricMaterial : public IMaterial
{
public:
    DielectricMaterial(float refraction_index) : m_refractionIndex(refraction_index) {}
    glm::vec3 scatter(const Ray& ray, HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override;
   
    glm::vec3 refract(const glm::vec3& uv, const glm::vec3& n, float etai_over_etat) const {
        float cos_theta = std::fmin(glm::dot(-uv, n), 1.0f);
        glm::vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
        glm::vec3 r_out_parallel = -std::sqrt(std::fabs(1.0f - glm::length2(r_out_perp))) * n;
        return r_out_perp + r_out_parallel;
    }

    IMaterial* clone() const override {
        return new DielectricMaterial(m_refractionIndex);
    }

private:
    float m_refractionIndex;

    static double reflectance(double cosine, double refraction_index)
    {
        double r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};