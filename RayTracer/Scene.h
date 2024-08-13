#pragma once
#include <vector>
#include "Sphere.h"
#include "Camera.h"

class Scene
{
public:
	Scene();

	void render();

private:
	std::vector<Sphere> spheres;

	void writeColor(const glm::vec3& color) const;

	glm::vec3 rayColor(const Ray& ray, const std::vector<Sphere>& scene) const;
};

