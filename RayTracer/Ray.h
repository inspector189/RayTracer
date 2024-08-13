#pragma once
#include <glm.hpp>

class Ray
{
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction);

    glm::vec3 at(float t) const;
    glm::vec3 getDirection() const;
    glm::vec3 getOrigin() const;

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};