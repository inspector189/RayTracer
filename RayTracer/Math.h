#pragma once
#include <random>

template <class T>
inline T Random();

template <>
inline float Random<float>() {
    static std::uniform_real_distribution<float> unif(0.0, 1.0);
    static std::default_random_engine rng;
    return unif(rng);
}

template <>
inline int Random<int>() {
    static std::uniform_int_distribution<int> uni(0, 1);
    static std::default_random_engine rng;
    return uni(rng);
}

template <class T>
inline T Random(T min, T max) {
    return min + (max - min) * Random<T>();
}

glm::vec3 Camera::RandomInUnitDisk() const {
    while (true) {
        const glm::vec3 p = glm::vec3(Random(-1.0f, 1.0f), Random(-1.0f, 1.0f), 0);
        if (glm::length2(p) >= 1.0)
            continue;
        return p;
    }
}