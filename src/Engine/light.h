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

        // A default constructed light is black, so it does not contribute anything.
        alignas(16) glm::vec3 position{0.0f};
        float radius = 0.0f;
        alignas(16) glm::vec3 color{0.0f};
        float intensity = 0.0f;

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
