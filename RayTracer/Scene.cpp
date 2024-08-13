#include "Scene.h"
#include<iostream>

Scene::Scene()
{
	spheres.emplace_back(glm::vec3(0.f, 0.f, -1.f), 0.5f);
	spheres.emplace_back(glm::vec3(0.f, -100.5f, -1.f), 100.f);
}

void Scene::render()
{
    const Camera camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f), 90.f, 16.f / 9.f, 0.01f, 1.f);
    
    const float aspectRatio = 16.f / 9.f;
    const int imageWidth = 400;
    const int imageHeight = int(imageWidth / aspectRatio);

    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; j--)
    {
        for (int i = 0; i < imageWidth; i++)
        {
            const float u = float(i)/float(imageWidth);
            const float v = float(j)/float(imageHeight);
            const Ray ray = camera.generateRay(u, v);
            const glm::vec3 color = rayColor(ray, spheres);
            writeColor(color);
        }
    }
}

void Scene::writeColor(const glm::vec3& color) const
{
    int r = int(255.999 * color.x);
    int g = int(255.999 * color.y);
    int b = int(255.999 * color.z);
    std::cout << r << " " << g << " " << b << std::endl;
}


glm::vec3 Scene::rayColor(const Ray& ray, const std::vector<Sphere>& scene) const
{
    HitRecord hitRecord;
    bool hitAnything = false;
    hitRecord.m_time = 10000.f;

    for (int i = 0; i < scene.size(); i++)
    {
        if (scene[i].hit(ray, 0.f, hitRecord.m_time, &hitRecord))
        {
            hitAnything = true;
        }
    }

    if (hitAnything)
    {
        return 0.5f * (hitRecord.m_normal + glm::vec3(1.f, 1.f, 1.f));
    }

    glm::vec3 unitDirection = glm::normalize(ray.getDirection());
    float a = 0.5f * (unitDirection.y + 1.f);
    return (1.f - a) * glm::vec3(1.f, 1.f, 1.f) + a * glm::vec3(0.5f, 0.7f, 1.f);
}
