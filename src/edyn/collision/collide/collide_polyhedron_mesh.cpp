#include "edyn/collision/collide.hpp"
#include "edyn/collision/collision_result.hpp"
#include "edyn/config/constants.hpp"
#include "edyn/math/vector3.hpp"
#include "edyn/util/triangle_util.hpp"
#include "edyn/util/shape_util.hpp"
#include "edyn/math/vector2_3_util.hpp"
#include "edyn/math/math.hpp"
#include "edyn/math/transform.hpp"

namespace edyn {

static void collide_polyhedron_triangle(
    const polyhedron_shape &poly, const triangle_mesh &mesh, size_t tri_idx,
    const collision_context &ctx, collision_result &result) {

    // The triangle vertices are shifted by the polyhedron's position so all
    // calculations are effectively done with the polyhedron in the origin.
    // The rotated mesh is used thus no rotations are necessary.
    const auto &pos_poly = ctx.posA;
    const auto &orn_poly = ctx.ornA;
    const auto &rmesh = *poly.rotated;

    auto tri_vertices_original = mesh.get_triangle_vertices(tri_idx);
    auto tri_normal = mesh.get_triangle_normal(tri_idx);

    // Shift vertices into A's positional object space.
    auto tri_vertices = tri_vertices_original;
    for (auto &v : tri_vertices) {
        v -= pos_poly;
    }

    auto sep_axis = vector3_zero;
    auto distance = -EDYN_SCALAR_MAX;
    auto projection_tri = scalar{};
    auto projection_poly = scalar{};

    // Polyhedron face normals.
    for (size_t i = 0; i < rmesh.relevant_normals.size(); ++i) {
        auto dir = -rmesh.relevant_normals[i]; // Point towards polyhedron.
        auto &poly_vertex = rmesh.vertices[poly.mesh->relevant_indices[i]];

        auto proj_poly = dot(poly_vertex, dir);
        auto proj_tri = get_triangle_support_projection(tri_vertices, dir);
        auto dist = proj_poly - proj_tri;

        if (dist > distance) {
            distance = dist;
            sep_axis = dir;
            projection_tri = proj_tri;
            projection_poly = proj_poly;
        }
    }

    // Triangle face normal.
    {
        // Find point on polyhedron that's furthest along the opposite direction
        // of the triangle normal.
        auto proj_poly = -point_cloud_support_projection(rmesh.vertices, -tri_normal);
        auto proj_tri = dot(tri_vertices[0], tri_normal);
        auto dist = proj_poly - proj_tri;

        if (dist > distance) {
            distance = dist;
            sep_axis = tri_normal;
            projection_tri = proj_tri;
            projection_poly = proj_poly;
        }
    }

    auto add_direction = [&](vector3 dir) {
        auto proj_poly = -point_cloud_support_projection(rmesh.vertices, -dir);
        auto proj_tri = get_triangle_support_projection(tri_vertices, dir);
        auto dist = proj_poly - proj_tri;

        if (dist > distance) {
            distance = dist;
            sep_axis = dir;
            projection_tri = proj_tri;
            projection_poly = proj_poly;
        }
    };

    // Edge vs edge.
    for (auto &poly_edge : rmesh.relevant_edges) {
        for (size_t j = 0; j < 3; ++j) {
            auto v0 = tri_vertices[j];
            auto v1 = tri_vertices[(j + 1) % 3];
            auto tri_edge = v1 - v0;
            auto dir = cross(poly_edge, tri_edge);

            if (!try_normalize(dir)) {
                continue;
            }

            add_direction(dir);
            add_direction(-dir);
        }
    }

    if (distance > ctx.threshold) {
        return;
    }

    triangle_feature tri_feature;
    size_t tri_feature_index;
    scalar proj_tri;
    get_triangle_support_feature(tri_vertices, vector3_zero, sep_axis,
                                 tri_feature, tri_feature_index,
                                 proj_tri, support_feature_tolerance);

    sep_axis = clip_triangle_separating_axis(sep_axis, mesh, tri_idx, tri_vertices, tri_normal, tri_feature, tri_feature_index);

    get_triangle_support_feature(tri_vertices, vector3_zero, sep_axis,
                                 tri_feature, tri_feature_index,
                                 proj_tri, support_feature_tolerance);

    projection_poly = -point_cloud_support_projection(rmesh.vertices, -sep_axis);

    distance = projection_poly - proj_tri;

    if (distance > ctx.threshold) {
        return;
    }

    auto polygon = point_cloud_support_polygon(
        rmesh.vertices.begin(), rmesh.vertices.end(), vector3_zero,
        sep_axis, projection_poly, true, support_feature_tolerance);

    auto contact_origin_tri = sep_axis * projection_tri;
    auto hull_tri = std::array<size_t, 3>{};
    size_t hull_tri_size = 0;

    switch (tri_feature) {
    case triangle_feature::face:
        std::iota(hull_tri.begin(), hull_tri.end(), 0);
        hull_tri_size = 3;
        break;
    case triangle_feature::edge:
        hull_tri[0] = tri_feature_index;
        hull_tri[1] = (tri_feature_index + 1) % 3;
        hull_tri_size = 2;
        break;
    case triangle_feature::vertex:
        hull_tri[0] = tri_feature_index;
        hull_tri_size = 1;
    }

    auto plane_vertices_tri = std::array<vector2, 3>{};
    for (size_t i = 0; i < 3; ++i) {
        auto &vertex = tri_vertices[i];
        auto vertex_tangent = to_object_space(vertex, contact_origin_tri, polygon.basis);
        auto vertex_plane = to_vector2_xz(vertex_tangent);
        plane_vertices_tri[i] = vertex_plane;
    }

    collision_result::collision_point point;
    point.normal = sep_axis;
    point.distance = distance;
    point.featureB = {tri_feature};
    point.featureB->index = get_triangle_mesh_feature_index(mesh, tri_idx, tri_feature, tri_feature_index);
    point.normal_attachment = contact_normal_attachment::none;

    // If the closest triangle feature is its face, check if the vertices of the
    // convex hull of the closest vertices of the polyhedron lie within the
    // triangle.
    if (tri_feature == triangle_feature::face) {
        point.normal_attachment = contact_normal_attachment::normal_on_B;

        for (auto idxA : polygon.hull) {
            auto &pointA = polygon.vertices[idxA];

            if (point_in_triangle(tri_vertices, sep_axis, pointA)) {
                point.pivotA = to_object_space(pointA, vector3_zero, orn_poly);
                point.pivotB = project_plane(pointA, contact_origin_tri, sep_axis) + pos_poly;
                result.maybe_add_point(point);
            }
        }
    } else if (polygon.hull.size() > 2) {
       point.normal_attachment = contact_normal_attachment::normal_on_A;
    }

    // If the boundary points of the polyhedron from a polygon (i.e. more than
    // 2 points) add contact points for the vertices of closest triangle feature
    // that lie inside of it.
    if (polygon.hull.size() > 2) {

        for (size_t i = 0; i < hull_tri_size; ++i) {
            auto idxB = hull_tri[i];
            auto &pointB = tri_vertices[idxB];

            if (point_in_polygonal_prism(polygon.vertices, polygon.hull, sep_axis, pointB)) {
                auto pivotA_world = project_plane(pointB, polygon.origin, sep_axis);
                point.pivotA = to_object_space(pivotA_world, vector3_zero, orn_poly);
                point.pivotB = tri_vertices_original[idxB];
                result.maybe_add_point(point);
            }
        }
    }

    // Calculate 2D intersection of edges on the closest features.
    if (polygon.hull.size() > 1 && hull_tri_size > 1) {
        // If the feature is a polygon, it will be necessary to wrap around the
        // vertex array. If it is just one edge, then avoid calculating the same
        // segment-segment intersection twice.
        const auto size_poly = polygon.hull.size();
        const auto limit_poly = size_poly == 2 ? 1 : size_poly;
        const auto limit_tri = hull_tri_size == 2 ? 1 : hull_tri_size;
        scalar s[2], t[2];

        for (size_t i = 0; i < limit_poly; ++i) {
            auto idx0A = polygon.hull[i];
            auto idx1A = polygon.hull[(i + 1) % size_poly];
            auto &v0A = polygon.plane_vertices[idx0A];
            auto &v1A = polygon.plane_vertices[idx1A];

            for (size_t j = 0; j < limit_tri; ++j) {
                auto idx0B = hull_tri[j];
                auto idx1B = hull_tri[(j + 1) % hull_tri_size];
                auto &v0B = plane_vertices_tri[idx0B];
                auto &v1B = plane_vertices_tri[idx1B];
                auto num_points = intersect_segments(v0A, v1A, v0B, v1B,
                                                     s[0], t[0], s[1], t[1]);

                for (size_t k = 0; k < num_points; ++k) {
                    auto pivotA_world = lerp(polygon.vertices[idx0A], polygon.vertices[idx1A], s[k]);
                    point.pivotA = to_object_space(pivotA_world, vector3_zero, orn_poly);
                    point.pivotB = lerp(tri_vertices_original[idx0B], tri_vertices_original[idx1B], t[k]);
                    result.maybe_add_point(point);
                }
            }
        }
    }
}

void collide(const polyhedron_shape &poly, const triangle_mesh &mesh,
             const collision_context &ctx, collision_result &result) {
    const auto inset = vector3_one * -contact_breaking_threshold;
    const auto visit_aabb = ctx.aabbA.inset(inset);

    mesh.visit_triangles(visit_aabb, [&](auto tri_idx) {
        collide_polyhedron_triangle(poly, mesh, tri_idx, ctx, result);
    });
}

}
