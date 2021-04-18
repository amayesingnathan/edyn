#ifndef EDYN_COLLISION_COLLIDE_HPP
#define EDYN_COLLISION_COLLIDE_HPP

#include <optional>
#include <functional>
#include "edyn/comp/shape.hpp"
#include "edyn/comp/rotated_mesh.hpp"
#include "edyn/collision/collision_result.hpp"
#include "edyn/math/quaternion.hpp"
#include "edyn/shapes/compound_shape.hpp"
#include "edyn/util/aabb_util.hpp"

namespace edyn {

struct collision_context {
    vector3 posA;
    quaternion ornA;
    AABB aabbA;
    vector3 posB;
    quaternion ornB;
    AABB aabbB;
    scalar threshold;

    using rotated_mesh_opt_ref = std::optional<std::reference_wrapper<rotated_mesh>>;
    rotated_mesh_opt_ref rmeshA;
    rotated_mesh_opt_ref rmeshB;

    collision_context swapped() const {
        return {posB, ornB, aabbB,
                posA, ornA, aabbA,
                threshold,
                rmeshB, rmeshA};
    }
};

// Calls `collide` with the `shA` and `shB` parameters swapped and swaps
// the returned result so the contact pivots and normals match up with the
// order of shapes A and B.
template<typename ShapeAType, typename ShapeBType>
void swap_collide(const ShapeAType &shA, const ShapeBType &shB,
                  const collision_context &ctx, collision_result &result);

// Sphere-Sphere
void collide(const sphere_shape &shA, const sphere_shape &shB,
             const collision_context &ctx, collision_result &result);

// Plane-Plane
inline
void collide(const plane_shape &shA, const plane_shape &shB,
                         const collision_context &ctx, collision_result &result) {
    // collision between infinite planes is undefined here.
}

// Sphere-Plane
void collide(const sphere_shape &shA, const plane_shape &shB,
             const collision_context &ctx, collision_result &result);

// Plane-Sphere
void collide(const plane_shape &shA, const sphere_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Cylinder
void collide(const cylinder_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Plane
void collide(const cylinder_shape &shA, const plane_shape &shB,
             const collision_context &ctx, collision_result &result);

// Plane-Cylinder
void collide(const plane_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Sphere
void collide(const cylinder_shape &shA, const sphere_shape &shB,
             const collision_context &ctx, collision_result &result);

// Sphere-Cylinder
void collide(const sphere_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Capsule
void collide(const capsule_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Plane
void collide(const capsule_shape &shA, const plane_shape &shB,
             const collision_context &ctx, collision_result &result);

// Plane-Capsule
void collide(const plane_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Sphere
void collide(const capsule_shape &shA, const sphere_shape &shB, 
             const collision_context &ctx, collision_result &result);

// Sphere-Capsule
void collide(const sphere_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Cylinder
void collide(const capsule_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Capsule
void collide(const cylinder_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Mesh-Mesh
inline
void collide(const mesh_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result) {
    // collision between triangle meshes still undefined.
}

// Plane-Mesh
inline
void collide(const plane_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result) {
    // collision between triangle meshes and planes still undefined.
}

// Mesh-Plane
inline
void collide(const mesh_shape &shA, const plane_shape &shB,
             const collision_context &ctx, collision_result &result) {
    swap_collide(shA, shB, ctx, result);
}

// Sphere-Mesh
void collide(const sphere_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Mesh-Sphere
void collide(const mesh_shape &shA, const sphere_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Mesh
void collide(const capsule_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Mesh-Capsule
void collide(const mesh_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Mesh
void collide(const cylinder_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Mesh-Cylinder
void collide(const mesh_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Box
void collide(const box_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Plane
void collide(const box_shape &shA, const plane_shape &shB,
             const collision_context &ctx, collision_result &result);

// Plane-Box
void collide(const plane_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Sphere-Box
void collide(const sphere_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Sphere
void collide(const box_shape &shA, const sphere_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Box
void collide(const capsule_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Capsule
void collide(const box_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Box
void collide(const cylinder_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Cylinder
void collide(const box_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Mesh
void collide(const box_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Mesh-Box
void collide(const mesh_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Paged Mesh-Paged Mesh
inline
void collide(const paged_mesh_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result) {
    // collision between paged triangle meshes still undefined.
}

// Plane-Paged Mesh
inline
void collide(const plane_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result) {
    // collision between paged triangle meshes and planes still undefined.
}

// Paged Mesh-Plane
inline
void collide(const paged_mesh_shape &shA, const plane_shape &shB,
             const collision_context &ctx, collision_result &result) {
    swap_collide(shA, shB, ctx, result);
}

// Sphere-Paged Mesh
void collide(const sphere_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Paged Mesh-Sphere
void collide(const paged_mesh_shape &shA, const sphere_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Paged Mesh
void collide(const capsule_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Paged Mesh-Capsule
void collide(const paged_mesh_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Paged Mesh
void collide(const cylinder_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Paged Mesh-Cylinder
void collide(const paged_mesh_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Paged Mesh
void collide(const box_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Paged Mesh-Box
void collide(const paged_mesh_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Mesh-Paged Mesh
inline
void collide(const mesh_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result) {
    // collision between triangle meshes still undefined.
}

// Paged Mesh-Mesh
inline
void collide(const paged_mesh_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result) {
    swap_collide(shA, shB, ctx, result);
}

// Sphere-Triangle
void collide_sphere_triangle(
    const sphere_shape &, const vector3 &sphere_pos, const quaternion &sphere_orn,
    const triangle_shape &tri, scalar threshold, collision_result &result);

// Cylinder-Triangle
void collide_cylinder_triangle(
    const cylinder_shape &, const vector3 &posA, const quaternion &ornA,
    const vector3 &disc_center_pos, const vector3 &disc_center_neg,
    const vector3 &cylinder_axis, const triangle_shape &tri, 
    scalar threshold, collision_result &result);

// Capsule-Triangle
void collide_capsule_triangle(
    const capsule_shape &capsule, const vector3 &posA, const quaternion &ornA,
    const std::array<vector3, 2> &capsule_vertices, 
    const triangle_shape &tri, scalar threshold, collision_result &result);

// Box-Triangle
void collide_box_triangle(
    const box_shape &, const vector3 &box_pos, const quaternion &box_orn,
    const std::array<vector3, 3> box_axes, const triangle_shape &tri,
    scalar threshold, collision_result &result);

// Polyhedron-Polyhedron
void collide(const polyhedron_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Plane
void collide(const polyhedron_shape &shA, const plane_shape &shB,
             const collision_context &ctx, collision_result &result);

// Plane-Polyhedron
void collide(const plane_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Sphere-Polyhedron
void collide(const sphere_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Sphere
void collide(const polyhedron_shape &shA, const sphere_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Polyhedron
void collide(const box_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Box
void collide(const polyhedron_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Polyhedron
void collide(const capsule_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Capsule
void collide(const polyhedron_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Polyhedron
void collide(const cylinder_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Cylinder
void collide(const polyhedron_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Mesh
void collide(const polyhedron_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Mesh-Polyhedron
void collide(const mesh_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Paged Mesh
void collide(const polyhedron_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Paged Mesh-Polyhedron
void collide(const paged_mesh_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Triangle
void collide_polyhedron_triangle(
    const polyhedron_shape &, const rotated_mesh &,
    const vector3 &pos_poly, const quaternion &orn_poly,
    const triangle_shape &tri, scalar threshold, collision_result &result);

// Compound-Compound
void collide(const compound_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);

// Compound-Plane
void collide(const compound_shape &shA, const plane_shape &shB,
             const collision_context &ctx, collision_result &result);

// Plane-Compound
void collide(const plane_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);

/*
// Sphere-Compound
void collide(const sphere_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);

// Compound-Sphere
void collide(const compound_shape &shA, const sphere_shape &shB,
             const collision_context &ctx, collision_result &result);

// Box-Compound
void collide(const box_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);

// Compound-Box
void collide(const compound_shape &shA, const box_shape &shB,
             const collision_context &ctx, collision_result &result);

// Capsule-Compound
void collide(const capsule_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);

// Compound-Capsule
void collide(const compound_shape &shA, const capsule_shape &shB,
             const collision_context &ctx, collision_result &result);

// Cylinder-Compound
void collide(const cylinder_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);

// Compound-Cylinder
void collide(const compound_shape &shA, const cylinder_shape &shB,
             const collision_context &ctx, collision_result &result);

// Compound-Polyhedron
void collide(const compound_shape &shA, const polyhedron_shape &shB,
             const collision_context &ctx, collision_result &result);

// Polyhedron-Compound
void collide(const polyhedron_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);

// Compound-Mesh
void collide(const compound_shape &shA, const mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Mesh-Compound
void collide(const mesh_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);

// Compound-Paged Mesh
void collide(const compound_shape &shA, const paged_mesh_shape &shB,
             const collision_context &ctx, collision_result &result);

// Paged Mesh-Compound
void collide(const paged_mesh_shape &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result);
 */
template<typename ShapeAType, typename ShapeBType>
void swap_collide(const ShapeAType &shA, const ShapeBType &shB,
                  const collision_context &ctx, collision_result &result) {
    collide(shB, shA, ctx.swapped(), result);
    result.swap(ctx.ornB, ctx.ornA);
}

template<typename T>
void collide(const compound_shape &shA, const T &shB,
             const collision_context &ctx, collision_result &result) {
    
    auto posB_in_A = to_object_space(ctx.posB, ctx.posA, ctx.ornA);
    auto ornB_in_A = conjugate(ctx.ornA) * ctx.ornB;
    auto aabbB_in_A = aabb_of_aabb(ctx.aabbB, posB_in_A, ornB_in_A);

    shA.visit(aabbB_in_A, [&] (auto && sh, const vector3 &pos, const quaternion &orn) {
        auto inner_ctx = collision_context{};
        inner_ctx.posA = pos;
        inner_ctx.ornA = orn;
        inner_ctx.posB = posB_in_A;
        inner_ctx.ornB = ornB_in_A;
        inner_ctx.threshold = ctx.threshold;
        collide(sh, shB, inner_ctx, result);
    });
}

template<typename T>
void collide(const T &shA, const compound_shape &shB,
             const collision_context &ctx, collision_result &result) {
    swap_collide(shA, shB, ctx, result);
}

}

#endif // EDYN_COLLISION_COLLIDE_HPP
