#include "edyn/collision/collide.hpp"

namespace edyn {

collision_result collide(const capsule_shape &shA, const sphere_shape &shB, 
                         const collision_context &ctx) {
    // TODO
    return {};
}

collision_result collide(const sphere_shape &shA, const capsule_shape &shB,
                         const collision_context &ctx) {
    return swap_collide(shA, shB, ctx);
}

}
