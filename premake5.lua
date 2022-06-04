project "edyn"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "on"

    targetdir 	("%{wks.location}/bin/%{prj.name}/" .. outputDir)
    objdir 		("%{wks.location}/obj/%{prj.name}/" .. outputDir)

	files
	{
		"include/**.h",
		"include/**.hpp",
        "src/edyn/math/geom.cpp",
        "src/edyn/math/quaternion.cpp",
        "src/edyn/collision/broadphase_main.cpp",
        "src/edyn/collision/broadphase_worker.cpp",
        "src/edyn/collision/narrowphase.cpp",
        "src/edyn/collision/contact_manifold_map.cpp",
        "src/edyn/collision/dynamic_tree.cpp",
        "src/edyn/collision/collide/collide_sphere_sphere.cpp",
        "src/edyn/collision/collide/collide_sphere_plane.cpp",
        "src/edyn/collision/collide/collide_cylinder_cylinder.cpp",
        "src/edyn/collision/collide/collide_cylinder_plane.cpp",
        "src/edyn/collision/collide/collide_cylinder_sphere.cpp",
        "src/edyn/collision/collide/collide_capsule_capsule.cpp",
        "src/edyn/collision/collide/collide_capsule_cylinder.cpp",
        "src/edyn/collision/collide/collide_capsule_plane.cpp",
        "src/edyn/collision/collide/collide_capsule_sphere.cpp",
        "src/edyn/collision/collide/collide_capsule_mesh.cpp",
        "src/edyn/collision/collide/collide_box_box.cpp",
        "src/edyn/collision/collide/collide_box_plane.cpp",
        "src/edyn/collision/collide/collide_capsule_box.cpp",
        "src/edyn/collision/collide/collide_cylinder_box.cpp",
        "src/edyn/collision/collide/collide_sphere_box.cpp",
        "src/edyn/collision/collide/collide_cylinder_mesh.cpp",
        "src/edyn/collision/collide/collide_sphere_mesh.cpp",
        "src/edyn/collision/collide/collide_box_mesh.cpp",
        "src/edyn/collision/collide/collide_polyhedron_capsule.cpp",
        "src/edyn/collision/collide/collide_polyhedron_cylinder.cpp",
        "src/edyn/collision/collide/collide_polyhedron_box.cpp",
        "src/edyn/collision/collide/collide_polyhedron_plane.cpp",
        "src/edyn/collision/collide/collide_polyhedron_polyhedron.cpp",
        "src/edyn/collision/collide/collide_polyhedron_sphere.cpp",
        "src/edyn/collision/collide/collide_polyhedron_mesh.cpp",
        "src/edyn/collision/collide/collide_compound_compound.cpp",
        "src/edyn/collision/collide/collide_compound_plane.cpp",
        "src/edyn/collision/collide/collide_compound_mesh.cpp",
        "src/edyn/collision/should_collide.cpp",
        "src/edyn/collision/collision_result.cpp",
        "src/edyn/collision/raycast.cpp",
        "src/edyn/constraints/contact_constraint.cpp",
        "src/edyn/constraints/distance_constraint.cpp",
        "src/edyn/constraints/soft_distance_constraint.cpp",
        "src/edyn/constraints/point_constraint.cpp",
        "src/edyn/constraints/hinge_constraint.cpp",
        "src/edyn/constraints/generic_constraint.cpp",
        "src/edyn/constraints/cvjoint_constraint.cpp",
        "src/edyn/constraints/cone_constraint.cpp",
        "src/edyn/constraints/null_constraint.cpp",
        "src/edyn/constraints/gravity_constraint.cpp",
        "src/edyn/dynamics/solver.cpp",
        "src/edyn/dynamics/restitution_solver.cpp",
        "src/edyn/sys/update_aabbs.cpp",
        "src/edyn/sys/update_rotated_meshes.cpp",
        "src/edyn/sys/update_inertias.cpp",
        "src/edyn/sys/update_presentation.cpp",
        "src/edyn/sys/update_origins.cpp",
        "src/edyn/util/rigidbody.cpp",
        "src/edyn/util/constraint_util.cpp",
        "src/edyn/util/shape_util.cpp",
        "src/edyn/util/aabb_util.cpp",
        "src/edyn/util/moment_of_inertia.cpp",
        "src/edyn/util/shape_volume.cpp",
        "src/edyn/util/collision_util.cpp",
        "src/edyn/shapes/triangle_mesh.cpp",
        "src/edyn/shapes/paged_triangle_mesh.cpp",
        "src/edyn/util/triangle_util.cpp",
        "src/edyn/util/ragdoll.cpp",
        "src/edyn/util/exclude_collision.cpp",
        "src/edyn/util/make_reg_op_builder.cpp",
        "src/edyn/shapes/box_shape.cpp",
        "src/edyn/shapes/cylinder_shape.cpp",
        "src/edyn/shapes/polyhedron_shape.cpp",
        "src/edyn/shapes/convex_mesh.cpp",
        "src/edyn/shapes/compound_shape.cpp",
        "src/edyn/parallel/entity_graph.cpp",
        "src/edyn/parallel/job_queue.cpp",
        "src/edyn/parallel/job_dispatcher.cpp",
        "src/edyn/parallel/job_scheduler.cpp",
        "src/edyn/parallel/job_queue_scheduler.cpp",
        "src/edyn/parallel/island_worker.cpp",
        "src/edyn/parallel/island_coordinator.cpp",
        "src/edyn/parallel/island_worker_context.cpp",
        "src/edyn/parallel/map_child_entity.cpp",
        "src/edyn/serialization/paged_triangle_mesh_s11n.cpp",
        "src/edyn/networking/context/client_network_context.cpp",
        "src/edyn/networking/context/server_network_context.cpp",
        "src/edyn/networking/sys/server_side.cpp",
        "src/edyn/networking/sys/client_side.cpp",
        "src/edyn/networking/networking.cpp",
        "src/edyn/networking/sys/update_aabbs_of_interest.cpp",
        "src/edyn/networking/sys/update_network_dirty.cpp",
        "src/edyn/networking/extrapolation_job.cpp",
        "src/edyn/networking/util/pool_snapshot.cpp",
        "src/edyn/networking/util/clock_sync.cpp",
        "src/edyn/networking/util/process_update_entity_map_packet.cpp",
        "src/edyn/networking/networking_external.cpp",
        "src/edyn/context/settings.cpp",
        "src/edyn/edyn.cpp",
        "src/edyn/time/common/time.cpp"
	}
	
	includedirs
	{
		"include",
        "%{IncludeDir.entt}"
	}

    filter "system:windows"
        systemversion "latest"
		links "winmm.lib"
		buildoptions { "/bigobj" }
		files "src/edyn/time/windows/time.cpp"
        
    filter "system:linux"
		pic "On"
		links "pthread"
		files "src/edyn/time/unix/time.cpp"

    filter "configurations:x64d"
		runtime "Debug"
        symbols "on"

	filter "configurations:ARMd"
		runtime "Debug"
        symbols "on"

	filter "configurations:ARM64d"
		runtime "Debug"
		symbols "on"

    filter "configurations:x64"
		runtime "Release"
        optimize "on"

	filter "configurations:ARM"
		runtime "Release"
		optimize "on"

	filter "configurations:ARM64"
		runtime "Release"
		optimize "on"
