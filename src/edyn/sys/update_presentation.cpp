#include "edyn/sys/update_presentation.hpp"
#include "edyn/comp/position.hpp"
#include "edyn/comp/present_position.hpp"
#include "edyn/comp/orientation.hpp"
#include "edyn/comp/present_orientation.hpp"
#include "edyn/comp/linvel.hpp"
#include "edyn/comp/angvel.hpp"
#include "edyn/comp/island.hpp"
#include "edyn/comp/tag.hpp"
#include "edyn/parallel/island_coordinator.hpp"
#include <entt/entity/registry.hpp>

namespace edyn {

void update_presentation(entt::registry &registry, double time) {
    auto &coordinator = registry.ctx<island_coordinator>();
    auto exclude = entt::exclude<sleeping_tag, disabled_tag>;
    auto linear_view = registry.view<position, linvel, present_position, island_worker_resident, procedural_tag>(exclude);
    auto angular_view = registry.view<orientation, angvel, present_orientation, island_worker_resident, procedural_tag>(exclude);
    constexpr double max_dt = 0.02;

    linear_view.each([&] (position &pos, linvel &vel, present_position &pre, island_worker_resident &resident) {
        EDYN_ASSERT(resident.worker_index != invalid_worker_index);
        auto worker_time = coordinator.get_worker_timestamp(resident.worker_index);
        auto dt = scalar(std::min(time - worker_time, max_dt));
        pre = pos + vel * dt;
    });

    angular_view.each([&] (orientation &orn, angvel &vel, present_orientation &pre, island_worker_resident &resident) {
        EDYN_ASSERT(resident.worker_index != invalid_worker_index);
        auto worker_time = coordinator.get_worker_timestamp(resident.worker_index);
        auto dt = scalar(std::min(time - worker_time, max_dt));
        pre = integrate(orn, vel, dt);
    });
}

void snap_presentation(entt::registry &registry) {
    auto view = registry.view<position, orientation, present_position, present_orientation>();
    view.each([] (position &pos, orientation &orn, present_position &p_pos, present_orientation &p_orn) {
        p_pos = pos;
        p_orn = orn;
    });
}

}