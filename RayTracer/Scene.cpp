#include "Scene.h"
#include "Math.h"
#include "Material.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
#include <limits>
#include <numbers>
#include <iomanip>

Scene::Scene()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    m_spheres.emplace_back(glm::vec3(0.f, -1000.f, 0.f), 1000.f, glm::vec3(0.5f, 0.5f, 0.5f), IMaterial::MaterialType::diffuse);
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = Random<float>();
            glm::vec3 center(a + 0.9f * Random<float>(), 0.2f, b + 0.9f * Random<float>());

            if (glm::length(center - glm::vec3(4, 0.2f, 0)) > 0.9f)
            {
                if (choose_mat < 0.8f)
                {
                    glm::vec3 albedo = RandomVec3() * RandomVec3();
                    m_spheres.emplace_back(center, 0.2f, albedo, IMaterial::MaterialType::diffuse);
                }
                else if (choose_mat < 0.95f)
                {
                    glm::vec3 albedo = RandomVec3(0.5f, 1.0f);
                    float fuzz = Random<float>(0.0f, 0.5f);
                    m_spheres.emplace_back(center, 0.2f, albedo, IMaterial::MaterialType::metal, fuzz);
                }
                else 
                {
                    m_spheres.emplace_back(center, 0.2f, glm::vec3(1.0f), IMaterial::MaterialType::dielectric, 1.5f);
                }
            }
        }
    }

    m_spheres.emplace_back(glm::vec3(0.f, 1.f, 0.f), 1.0f, glm::vec3(1.0f), IMaterial::MaterialType::dielectric, 1.5f);
    m_spheres.emplace_back(glm::vec3(-4.f, 1.f, 0.f), 1.0f, glm::vec3(0.4f, 0.2f, 0.1f), IMaterial::MaterialType::diffuse);
    m_spheres.emplace_back(glm::vec3(4.f, 1.f, 0.f), 1.0f, glm::vec3(0.7f, 0.6f, 0.5f), IMaterial::MaterialType::metal, 0.0f);
    /*m_spheres.emplace_back(glm::vec3(-1.2f, 3.0f, 0.f), 3.0f, glm::vec3(0.1f, 0.2f, 0.5f), IMaterial::MaterialType::diffuse);
    m_spheres.emplace_back(glm::vec3(-1.2f, 3.5f, -6.5f), 3.5f, glm::vec3(0.3f, 0.8f, 0.2f), IMaterial::MaterialType::metal, 0.6f);
    m_spheres.emplace_back(glm::vec3(-1.2f, 3.5f, 6.5f), 3.5f, glm::vec3(0.8f, 0.6f, 0.2f), IMaterial::MaterialType::dielectric, (1.0f / 1.5f));
    m_spheres.emplace_back(glm::vec3(1.0f, 3.0f, -13.0f), 3.0f, glm::vec3(0.8f, 0.4f, 0.5f), IMaterial::MaterialType::metal, 0.1f);
    m_spheres.emplace_back(glm::vec3(-1.2f, 2.0f, 13.0f), 2.0f, glm::vec3(0.5f, 0.6f, 0.4f), IMaterial::MaterialType::dielectric, 1.5f);*/
}
void Scene::render()
{
    const Camera camera(glm::vec3(13.f, 2.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 20.f, 16.f / 9.f, 0.01f, 10.f);

    const int samplesPerPixel = 500;
    const float aspectRatio = 16.f / 9.f;
    const int imageWidth = 800;
    const int imageHeight = int(imageWidth / aspectRatio);
    int total = imageWidth * imageHeight;
    int pixelIndex = 0;
    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
    std::cerr << "Starting render...\n";
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

            pixelIndex++;
            if (pixelIndex % 500 == 0 || pixelIndex == total) {
                float progress = 100.0f * pixelIndex / total;
                std::cerr << "\rProgress: " << std::fixed << std::setprecision(1) << progress << "%" << std::flush;
            }
        }
    }
    std::cerr << "\nDone.\n";
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
    hitRecord.m_time = 10000.f; // max
    bool hitAnything = false;
    const int maxDepth = 5;

    glm::vec3 attenuation(1.0f);

    if (depth >= maxDepth)
        return glm::vec3(0.f);

    for (const auto& sphere : m_spheres)
    {
        if (sphere.hit(ray, 0.0001f, hitRecord.m_time, &hitRecord))
        {
            hitAnything = true;
        }
    }

    if (!hitAnything)
    {
        // tło
        glm::vec3 unitDirection = glm::normalize(ray.getDirection());
        float a = 0.5f * (unitDirection.y + 1.f);
        return (1.f - a) * glm::vec3(1.f, 1.f, 1.f) + a * glm::vec3(0.5f, 0.7f, 1.f);
    }

    if (!hitRecord.material)
        return glm::vec3(0.f);  // bez materiału, czarny

    Ray scattered(ray.getOrigin(), ray.getDirection());
    hitRecord.material->scatter(ray, hitRecord, attenuation, scattered);
  //  attenuation = hitRecord.material->scatter(ray, hitRecord, attenuation, scattered);

    if (glm::length2(attenuation) > 1e-8f)
    {
        return attenuation * rayColor(scattered, depth + 1);
    }

    return glm::vec3(0.f);
}

