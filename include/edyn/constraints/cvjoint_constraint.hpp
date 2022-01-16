#ifndef EDYN_CONSTRAINTS_CVJOINT_CONSTRAINT_HPP
#define EDYN_CONSTRAINTS_CVJOINT_CONSTRAINT_HPP

#include <array>
#include <entt/entity/fwd.hpp>
#include "edyn/math/vector3.hpp"
#include "edyn/math/matrix3x3.hpp"
#include "edyn/constraints/constraint_base.hpp"
#include "edyn/constraints/prepare_constraints.hpp"
#include "edyn/util/array.hpp"

namespace edyn {

/**
 * @brief Constant-velocity joint.
 */
struct cvjoint_constraint : public constraint_base {
    // Pivots in object space.
    std::array<vector3, 2> pivot;

    // Frames in object space. A frame is an orthonormal basis. The first
    // column of the matrix is the twist axis. The other axes are used to
    // calculate the relative angle between frames.
    std::array<matrix3x3, 2> frame{matrix3x3_identity, matrix3x3_identity};

    // Twist angular limits. `angle_min` must be smaller than `angle_max`.
    scalar twist_min{}, twist_max{};

    // Angular limit restitution.
    scalar twist_restitution{};

    // Current relative angle between the two frames along the twist axis.
    // Do not modify.
    scalar twist_angle{};

    // When limits are set, if the angle between the two frames along the twist
    // axis is smaller than `twist_min + twist_bump_stop_angle` or greater than
    // `twist_max - twist_bump_stop_angle` a force proportional to the proximity
    // to the angular limit multiplied by `twist_bump_stop_stiffness` will be
    // applied.
    scalar twist_bump_stop_angle{};

    // Spring rate of bump stop for twisting in Nm/rad.
    scalar twist_bump_stop_stiffness{};

    // Torque applied in the opposite direction of the angular velocity along
    // the twist axis.
    scalar twist_friction_torque{};

    // Relative angle between the two frames along the twist axis that the
    // spring force should try to maintain.
    scalar twist_rest_angle{};

    // Spring stiffness for twisting in Nm/rad.
    scalar twist_stiffness{};

    // Damping rate for twisting in Nm/(rad/s).
    scalar twist_damping{};

    static constexpr auto num_rows = 7;
    std::array<scalar, num_rows> impulse {make_array<num_rows>(scalar{})};

    /**
     * @brief Recalculates the current angle. Should be called after changing
     * the constraint frames so that the relative angle is set correctly.
     * @param ornA Orientation of the first rigid body.
     * @param ornB Orientation of the second rigid body.
     */
    void reset_angle(const quaternion &ornA, const quaternion &ornB);

    scalar relative_angle(const quaternion &ornA, const quaternion &ornB) const;
    scalar relative_angle(const quaternion &ornA, const quaternion &ornB,
                          const vector3 &twist_axisA, const vector3 &twist_axisB) const;
    void update_angle(scalar new_angle);
};

template<>
void prepare_constraints<cvjoint_constraint>(entt::registry &, row_cache &, scalar dt);

template<>
bool solve_position_constraints<cvjoint_constraint>(entt::registry &, scalar dt);

template<typename Archive>
void serialize(Archive &archive, cvjoint_constraint &c) {
    archive(c.body, c.pivot, c.frame);
    archive(c.twist_min, c.twist_max);
    archive(c.twist_restitution);
    archive(c.twist_angle);
    archive(c.twist_bump_stop_angle);
    archive(c.twist_bump_stop_stiffness);
    archive(c.twist_friction_torque);
    archive(c.twist_rest_angle);
    archive(c.twist_stiffness);
    archive(c.twist_damping);
};

}

#endif // EDYN_CONSTRAINTS_CVJOINT_CONSTRAINT_HPP
