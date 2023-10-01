//
// Created by pbialas on 27.09.23.
//

#pragma once

#include "glad/gl.h"

#include "AbstractMaterial.h"

namespace xe {

    class KdMaterial : public AbstractMaterial<KdMaterial> {
    public:

        KdMaterial(glm::vec4 Kd, bool use_vertex_colors, GLuint texture) :
                Kd_{Kd}, use_vertex_colors_(use_vertex_colors), texture_(texture) {}

        KdMaterial(glm::vec4 Kd, bool use_vertex_colors) :
                KdMaterial(Kd, use_vertex_colors, 0) {}


        explicit KdMaterial(glm::vec4 Kd) : KdMaterial(Kd, false) {}


        static void init();

        void bind() const override;

        void unbind() const override;

    private:
        inline static GLint map_Kd_location_ = 0;
        glm::vec4 Kd_;
        int use_vertex_colors_;
        GLuint texture_;

    };

} // xe
