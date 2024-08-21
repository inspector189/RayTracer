#include "Math.h"

glm::vec3 RandomInUnitDisk()
{
    while (true) {
        const glm::vec3 p = glm::vec3(Random(-1.0f, 1.0f), Random(-1.0f, 1.0f), 0);
        if (glm::length2(p) >= 1.0)
            continue;
        return p;
    }
}
glm::vec3 RandomInUnitSphere()
{
    while (true) {
        auto p = glm::vec3(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f));
        if (glm::length2(p) < 1)
            return p;
    }
}

glm::vec3 RandomOnHemisphere(const glm::vec3& normal)
{
    glm::vec3 onUnitSphere = glm::normalize(RandomInUnitSphere());
    if (glm::dot(onUnitSphere, normal) > 0)
    {
        return onUnitSphere;
    }
    else
    {
        return -onUnitSphere;
    }
}