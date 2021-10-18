cmake_minimum_required(VERSION 3.17)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracer src/main.cpp src/geometry/point.h src/geometry/vector.h src/geometry/point.h src/geometry/arraybase.h src/geometry/ray.h src/rendering/camera.h src/scene/scene.h src/geometry/sphere.h src/geometry/triangle.h src/scene/TriangleModel.h src/material/diffuse.h src/material/spectral.h src/material/specular.h src/material/complex.h src/light/point.h src/rendering/render_pool.h src/rendering/ray_sampler.h src/rendering/framebuffer.h src/rendering/ray_generator.h src/cornell_box.h)

set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(lib/src/SFML)
target_compile_definitions(RayTracer PRIVATE SFML_STATIC)
target_include_directories(RayTracer PRIVATE ${CMAKE_SOURCE_DIR}/lib/src/SFML/include/)
target_link_libraries(RayTracer sfml-system sfml-network sfml-window sfml-graphics)

#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /arch:AVX")
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /permissive- /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise /errorReport:prompt /WX- /Zc:forScope /arch:AVX /Gd /Oi /MD /FC /EHsc /nologo /diagnostics:column")
