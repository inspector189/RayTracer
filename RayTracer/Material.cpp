#include <glm/gtc/epsilon.hpp>
#include "Material.h"
#include "Sphere.h"
#include "Math.h"


DiffuseMaterial::DiffuseMaterial(const glm::vec3& albedo)
{
	m_albedo = albedo;
}

glm::vec3 DiffuseMaterial::scatter(const Ray& ray, HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 direction = hit.m_normal + RandomInUnitSphere();
	if (glm::length2(direction) < 1e-8)
		direction = hit.m_normal;
	scattered = Ray(hit.m_point, direction);
	attenuation = m_albedo;
	return attenuation;;
}
MetalMaterial::MetalMaterial(const glm::vec3& albedo, double fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1.0 ? fuzz : 1.0) {};

bool near_zero(glm::vec3 e) {
	auto s = 1e-8;
	return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
}
glm::vec3 MetalMaterial::scatter(const Ray& ray, HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 reflected = reflect(glm::normalize(ray.getDirection()), hit.m_normal);
	reflected += m_fuzz * RandomInUnitSphere();
	scattered = Ray(hit.m_point, reflected);
	attenuation = m_albedo;

	if (glm::dot(scattered.getDirection(), hit.m_normal) > 0.0f)
		return attenuation;
	return glm::vec3(0.0f);
}

inline double random_double() {
	static std::mt19937 gen(std::random_device{}());
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(gen);
}

glm::vec3 DielectricMaterial::scatter(const Ray& ray, HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const
{
	attenuation = glm::vec3(1.0f, 1.0f, 1.0f);

	double ri = hit.front_face ? (1.0f / m_refractionIndex) : m_refractionIndex;

	glm::vec3 unit_direction = glm::normalize(ray.getDirection());

	float cos_theta = fmin(glm::dot(-unit_direction, hit.m_normal), 1.0f);
	float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
	
	bool cannot_refract = ri * sin_theta > 1.0f;
	glm::vec3 direction;
	if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
		direction = glm::reflect(unit_direction, hit.m_normal);
	}
	else {
		direction = refract(unit_direction, hit.m_normal, ri);
	}

	scattered = Ray(hit.m_point, direction);
	hit.scattered = scattered;
	hit.didScattered = true;

	return attenuation;

}