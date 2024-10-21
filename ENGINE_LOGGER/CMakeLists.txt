add_library(ENGINE_LOGGER
    "include/Logger/Logger.hpp"
    "include/Logger/Logger.inl"
    "src/Logger.cpp")

target_include_directories(
    ENGINE_LOGGER PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries( ENGINE_LOGGER PUBLIC fmt::fmt)

target_compile_options( 
    ENGINE_LOGGER PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)
