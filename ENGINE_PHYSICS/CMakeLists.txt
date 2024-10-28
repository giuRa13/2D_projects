add_library(ENGINE_PHYSICS
    "include/Physics/Box2DWrappers.hpp"
    "src/Box2DWrappers.cpp"
    )


target_include_directories(
    ENGINE_PHYSICS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    
target_link_libraries(ENGINE_PHYSICS
    PRIVATE ENGINE_LOGGER
    PUBLIC box2d::box2d)

target_compile_options(
    ENGINE_PHYSICS PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)