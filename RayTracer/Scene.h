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
	std::vector<Sphere> m_spheres;

	void writeColor(const glm::vec3& color, int samplesPerPixel) const;

	glm::vec3 rayColor(const Ray& ray, int depth) const;
};

