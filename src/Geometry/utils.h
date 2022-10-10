//
// Created by pbialas on 03.09.2020.
//


#pragma once

#include <limits>

#include "glm/glm.hpp"

namespace glm {
    template<typename glm::length_t L, typename F>
    glm::vec<L, F> max(const glm::vec<L, F> &lhs, const glm::vec<L, F> &rhs) {
        glm::vec<L, F> res;
        for (int i = 0; i < L; ++i)
            res[i] = std::max(lhs[i], rhs[i]);
        return res;
    }

    template<typename glm::length_t L, typename F>
    glm::vec<L, F> min(const glm::vec<L, F> &lhs, const glm::vec<L, F> &rhs) {
        glm::vec<L, F> res;
        for (int i = 0; i < L; ++i)
            res[i] = std::min(lhs[i], rhs[i]);
        return res;
    }
}

