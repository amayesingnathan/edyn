
#ifndef EDYN_PARALLEL_ENTITY_COMPONENT_CONTAINER_HPP
#define EDYN_PARALLEL_ENTITY_COMPONENT_CONTAINER_HPP

#include <vector>
#include <utility>
#include <entt/entity/registry.hpp>
#include "edyn/comp/dirty.hpp"
#include "edyn/parallel/merge/merge_component.hpp"
#include "edyn/parallel/merge/merge_contact_manifold.hpp"
#include "edyn/parallel/merge/merge_constraint.hpp"
#include "edyn/parallel/merge/merge_tree_view.hpp"
#include "edyn/parallel/merge/merge_collision_exclusion.hpp"

namespace edyn {

struct entity_component_container_base {
    virtual ~entity_component_container_base() {}
    virtual void import(entt::registry &, entity_map &, bool mark_dirty = false) = 0;
    virtual void reserve(size_t size) = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
};

// Only enable the updated component container for non-empty components since
// it doesn't make sense to update empty components because they have no data.
template<typename Component, std::enable_if_t<!std::is_empty_v<Component>, bool> = true>
struct updated_entity_component_container: public entity_component_container_base {
    std::vector<std::pair<entt::entity, Component>> pairs;

    void insert(entt::entity entity, const Component &comp) {
        pairs.emplace_back(entity, comp);
    }

    void import(entt::registry &registry, entity_map &emap, bool mark_dirty) override {
        for (auto &pair : pairs) {
            auto remote_entity = pair.first;
            if (!emap.has_rem(remote_entity)) continue;
            auto local_entity = emap.remloc(remote_entity);

            merge(pair.second, emap);
            registry.replace<Component>(local_entity, pair.second);

            if (mark_dirty) {
                registry.get_or_emplace<dirty>(local_entity).template updated<Component>();
            }
        }
    }

    void reserve(size_t size) override {
        pairs.reserve(size);
    }

    bool empty() const override {
        return pairs.empty();
    }

    void clear() override {
        pairs.clear();
    }
};

template<typename Component>
struct created_entity_component_container: public entity_component_container_base {
    std::vector<std::pair<entt::entity, Component>> pairs;

    void insert(entt::entity entity, const Component &comp) {
        pairs.emplace_back(entity, comp);
    }

    void import(entt::registry &registry, entity_map &emap, bool mark_dirty) override {
        size_t index = 0;

        while (index < pairs.size()) {
            auto &pair = pairs[index];
            auto remote_entity = pair.first;

            if (!emap.has_rem(remote_entity)) {
                ++index;
                continue;
            }

            auto local_entity = emap.remloc(remote_entity);

            // If it's a duplicate, remove it from the array by swapping with last
            // and popping last. This ensures no duplicates after processing, which
            // can be useful during post processing after import.
            if (registry.any_of<Component>(local_entity)) {
                pairs[index] = pairs.back();
                pairs.pop_back();
                continue;
            }

            if constexpr(std::is_empty_v<Component>) {
                registry.emplace<Component>(local_entity);
            } else {
                merge(pair.second, emap);
                registry.emplace<Component>(local_entity, pair.second);
            }

            if (mark_dirty) {
                registry.get_or_emplace<dirty>(local_entity).template created<Component>();
            }

            ++index;
        }
    }

    void reserve(size_t size) override {
        pairs.reserve(size);
    }

    bool empty() const override {
        return pairs.empty();
    }

    void clear() override {
        pairs.clear();
    }
};

template<typename Component>
struct destroyed_entity_component_container: public entity_component_container_base {
    std::vector<entt::entity> entities;

    void import(entt::registry &registry, entity_map &map, bool mark_dirty) override {
        for (auto remote_entity : entities) {
            if (!map.has_rem(remote_entity)) continue;
            auto local_entity = map.remloc(remote_entity);
            registry.remove<Component>(local_entity);

            if (mark_dirty) {
                registry.get_or_emplace<dirty>(local_entity).template destroyed<Component>();
            }
        }
    }

    void reserve(size_t size) override {
        entities.reserve(size);
    }

    bool empty() const override {
        return entities.empty();
    }

    void clear() override {
        entities.clear();
    }
};

}

#endif // EDYN_PARALLEL_ENTITY_COMPONENT_CONTAINER_HPP
