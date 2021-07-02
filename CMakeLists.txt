cmake_minimum_required(VERSION 3.17)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracer src/main.cpp src/geometry/point.h src/geometry/vector.h src/geometry/point.h src/geometry/arraybase.h src/geometry/ray.h src/scene/camera.h src/scene/scene.h src/geometry/sphere.h src/geometry/triangle.h src/scene/TriangleModel.h src/material/diffuse.h src/material/spectral.h src/material/specular.h src/material/complex.h src/light/point.h src/rendering/render.h)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_PREFIX "-d")
endif()

macro(link_sfml_static target)

    target_compile_definitions(${target} PRIVATE SFML_STATIC)
    target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/lib/x86-64/SFML-2.5.1/include)
    target_link_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/lib/x86-64/SFML-2.5.1/lib)
    target_link_libraries(${target} winmm.lib sfml-system-s${DEBUG_PREFIX}.lib ws2_32.lib sfml-network-s${DEBUG_PREFIX}.lib
        opengl32.lib gdi32.lib sfml-window-s${DEBUG_PREFIX}.lib freetype.lib sfml-graphics-s${DEBUG_PREFIX}.lib)
endmacro()

link_sfml_static(RayTracer)

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /arch:AVX")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /permissive- /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise /errorReport:prompt /WX- /Zc:forScope /arch:AVX /Gd /Oi /MD /FC /EHsc /nologo /diagnostics:column")
