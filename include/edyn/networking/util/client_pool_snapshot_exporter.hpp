#ifndef EDYN_NETWORKING_UTIL_CLIENT_POOL_SNAPSHOT_EXPORTER_HPP
#define EDYN_NETWORKING_UTIL_CLIENT_POOL_SNAPSHOT_EXPORTER_HPP

#include <entt/entity/registry.hpp>
#include "edyn/networking/util/pool_snapshot.hpp"

namespace edyn {

class client_pool_snapshot_exporter {
public:
    virtual ~client_pool_snapshot_exporter() = default;
    virtual void export_all(const entt::registry &registry,
                            const std::vector<entt::entity> &entities,
                            std::vector<pool_snapshot> &pools) = 0;
    virtual void export_transient(const entt::registry &registry,
                                  const std::vector<entt::entity> &entities,
                                  std::vector<pool_snapshot> &pools) = 0;
    virtual void export_input(const entt::registry &registry,
                              const std::vector<entt::entity> &entities,
                              std::vector<pool_snapshot> &pools) = 0;
    virtual void export_by_type_id(const entt::registry &registry,
                                   const std::vector<entt::entity> &entities,
                                   entt::entity entity, entt::id_type id,
                                   std::vector<pool_snapshot> &pools) = 0;
};

template<typename... Components>
class client_pool_snapshot_exporter_impl : public client_pool_snapshot_exporter {
public:
    using insert_entity_components_func_t = void(const entt::registry &, const std::vector<entt::entity> &, std::vector<pool_snapshot> &);
    insert_entity_components_func_t *insert_transient_entity_components_func;
    insert_entity_components_func_t *insert_input_entity_components_func;

    template<typename... Transient, typename... Input>
    client_pool_snapshot_exporter_impl(std::tuple<Components...>, std::tuple<Transient...>, std::tuple<Input...>) {
        static_assert((!std::is_empty_v<Input> && ...));

        insert_transient_entity_components_func = [] (const entt::registry &registry, const std::vector<entt::entity> &entities,
                                                      std::vector<pool_snapshot> &pools) {
            const std::tuple<Components...> components;
            internal::pool_insert_select_entity_components<Transient...>(registry, entities, pools, components);
        };
        insert_input_entity_components_func = [] (const entt::registry &registry, const std::vector<entt::entity> &entities,
                                                           std::vector<pool_snapshot> &pools) {
            const std::tuple<Components...> components;
            internal::pool_insert_select_entity_components<Input...>(registry, entities, pools, components);
        };
    }

    void export_all(const entt::registry &registry,
                    const std::vector<entt::entity> &entities,
                    std::vector<pool_snapshot> &pools) override {
        const std::tuple<Components...> components;
        internal::pool_insert_entity_components_all(registry, entities, pools, components,
                                                    std::make_index_sequence<sizeof...(Components)>{});
    }

    void export_transient(const entt::registry &registry,
                          const std::vector<entt::entity> &entities,
                          std::vector<pool_snapshot> &pools) override {
        (*insert_transient_entity_components_func)(registry, entities, pools);
    }

    void export_input(const entt::registry &registry,
                      const std::vector<entt::entity> &entities,
                      std::vector<pool_snapshot> &pools) override {
        (*insert_input_entity_components_func)(registry, entities, pools);
    }

    void export_by_type_id(const entt::registry &registry,
                           const std::vector<entt::entity> &entities,
                           entt::entity entity, entt::id_type id,
                           std::vector<pool_snapshot> &pools) override {
        size_t i = 0;
        ((entt::type_id<Components>().seq() == id ?
            internal::pool_insert_entity<Components>(registry, entity, entities, pools, i++) : (++i, void(0))), ...);
    }
};

}

#endif // EDYN_NETWORKING_UTIL_CLIENT_POOL_SNAPSHOT_EXPORTER_HPP
