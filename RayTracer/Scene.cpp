#include "Scene.h"
#include "Math.h"
#include<iostream>

Scene::Scene()
{
	m_spheres.emplace_back(glm::vec3(0.f, -1000.f, 0.f), 1000.f, glm::vec3(0.5f, 0.5f, 0.5f));
    for (int i = -11; i < 11; i++)
    {
        for (int j = -11; j < 11; j++)
        {
            glm::vec3 treshold(4.f, 0.2f, 0.f);
            glm::vec3 center(i + 0.9f * Random<float>(), 0.2f, j + 0.9f * Random<float>());
            if ((treshold - center).length() > 0.9f)
            {
                m_spheres.emplace_back(center, 0.2f, glm::vec3(Random<float>(), Random<float>(), Random<float>()));
            }
        }
    }
}

void Scene::render()
{
    const Camera camera(glm::vec3(13.f, 2.f, 3.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f), 90.f, 16.f / 9.f, 0.01f, 1.f);
    
    const int samplesPerPixel = 25;
    const float aspectRatio = 16.f / 9.f;
    const int imageWidth = 400;
    const int imageHeight = int(imageWidth / aspectRatio);

    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; j--)
    {
        for (int i = 0; i < imageWidth; i++)
        {
            glm::vec3 color(0.f, 0.f, 0.f);
            for (int k = 0; k < samplesPerPixel; k++)
            {
                const float u = float(i + Random<float>()) / float(imageWidth);
                const float v = float(j + Random<float>()) / float(imageHeight);
                const Ray ray = camera.generateRay(u, v);
                color += rayColor(ray, 0);
                
            }
            writeColor(color, samplesPerPixel);
        }
    }
}

void Scene::writeColor(const glm::vec3& color, int samplesPerPixel) const
{
    const float scale = 1.0f / samplesPerPixel;
    const float r = sqrtf(scale * color.x);
    const float g = sqrtf(scale * color.y);
    const float b = sqrtf(scale * color.z);
    std::cout << static_cast<int>(256 * glm::clamp(r, 0.0f, 0.999f)) << ' '
        << static_cast<int>(256 * glm::clamp(g, 0.0f, 0.999f)) << ' '
        << static_cast<int>(256 * glm::clamp(b, 0.0f, 0.999f)) << '\n';
}


glm::vec3 Scene::rayColor(const Ray& ray, int depth) const
{
    HitRecord hitRecord;
    bool hitAnything = false;
    hitRecord.m_time = 10000.f;
    const int maxDepth = 5;
    if (depth >= maxDepth)
    {
        return glm::vec3(0.f, 0.f, 0.f);
    }
    for (int i = 0; i < m_spheres.size(); i++)
    {
        if (m_spheres[i].hit(ray, 0.0001f, hitRecord.m_time, &hitRecord))
        {
            hitAnything = true;
        }
    }
    //funkcja ma miec maxDepth - po przekroczeniu koniec funkcji.
    if (hitAnything)
    {
        glm::vec3 direction = RandomOnHemisphere(hitRecord.m_normal);
        return hitRecord.m_attenuation * rayColor(Ray(hitRecord.m_point, direction), depth + 1);
    }
    glm::vec3 unitDirection = glm::normalize(ray.getDirection());
    float a = 0.5f * (unitDirection.y + 1.f);
    return (1.f - a) * glm::vec3(1.f, 1.f, 1.f) + a * glm::vec3(0.5f, 0.7f, 1.f);
}
