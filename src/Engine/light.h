//
// Created by pbialas on 05.10.23.
//


#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace xe {

    const GLuint MAX_POINT_LIGHTS = 16;
    struct PointLight {

        PointLight() = default;

        PointLight(const PointLight &) = default;

        PointLight(const glm::vec3 &pos, const glm::vec3 &color, float intensity, float radius)
                : position(pos), color(color), intensity(intensity), radius(radius) {}

        alignas(16) glm::vec3 position;
        float radius;
        alignas(16) glm::vec3 color;
        float intensity;

        void normalize() {
            color /= (color.r+color.g+color.b);
        }
    };



    inline PointLight transform(const PointLight &light, const glm::mat4 &M) {
        PointLight transformed_light(light);
        transformed_light.position = glm::vec3(M * glm::vec4(light.position, 1.0f));
        return transformed_light;
    }
}
