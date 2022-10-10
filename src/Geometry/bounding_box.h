//
// Created by pbialas on 03.09.2020.
//


#pragma once

#include <limits>

#include "glm/glm.hpp"

#include "Geometry/utils.h"

namespace xe {
    template<typename glm::length_t L, typename F = typename glm::vec3::value_type>
    class BoundingBox {
    public:
        using vec_t = glm::vec<L, F>;

        BoundingBox() : n_points_(0),
                        min_(std::numeric_limits<F>::max()),
                        max_(std::numeric_limits<F>::min()) {}

        void add(const vec_t &p) {
            n_points_++;
            min_ = glm::min(min_, p);
            max_ = glm::max(max_, p);
        }


        auto n_points() const { return n_points_; }

        auto min() const { return min_; }

        auto max() const { return max_; }


    private:
        size_t n_points_;
        vec_t min_;
        vec_t max_;
    };
}