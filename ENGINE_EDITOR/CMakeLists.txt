add_executable(ENGINE_EDITOR
    "src/main.cpp"
    "src/Application.cpp"
    "src/Application.hpp")

target_include_directories(
    ENGINE_EDITOR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    
target_link_libraries(ENGINE_EDITOR 
    PRIVATE ENGINE_WINDOW ENGINE_RENDERING ENGINE_LOGGER ENGINE_CORE ENGINE_SOUNDS ENGINE_PHYSICS
    PUBLIC glm::glm glad::glad soil2 EnTT::EnTT sol2 )

add_resources(TARGET Assets IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets OUT_DIR assets)
add_dependencies(ENGINE_EDITOR Copy-Assets)

set_target_properties(ENGINE_EDITOR PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/")

target_compile_options(
    ENGINE_EDITOR PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)