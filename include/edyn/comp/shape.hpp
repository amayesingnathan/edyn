#ifndef EDYN_COMP_SHAPE_HPP
#define EDYN_COMP_SHAPE_HPP

#include <variant>
#include "edyn/math/matrix3x3.hpp"
#include "edyn/shapes/plane_shape.hpp"
#include "edyn/shapes/sphere_shape.hpp"
#include "edyn/shapes/cylinder_shape.hpp"
#include "edyn/shapes/capsule_shape.hpp"
#include "edyn/shapes/mesh_shape.hpp"
#include "edyn/shapes/box_shape.hpp"
#include "edyn/shapes/polyhedron_shape.hpp"
#include "edyn/shapes/paged_mesh_shape.hpp"

namespace edyn {

struct shape {
    std::variant<plane_shape, 
                 sphere_shape, 
                 cylinder_shape,
                 capsule_shape,
                 mesh_shape,
                 box_shape,
                 polyhedron_shape,
                 paged_mesh_shape> var;

    matrix3x3 inertia(scalar mass) {
        matrix3x3 I;
        std::visit([&I, &mass] (auto &&s) {
            I = s.inertia(mass);
        }, var);
        return I;
    }
};

}

#endif // EDYN_COMP_SHAPE_HPP