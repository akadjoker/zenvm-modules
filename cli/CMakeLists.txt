cmake_minimum_required(VERSION 3.10)
project(zen_cli_modules LANGUAGES CXX)

# The batteries-included `zen`: the SAME cli/main.cpp as the zenvm core (no
# second copy), compiled with ZEN_CLI_EXTRA_LIBS_HEADER pointing at
# zen_modules_libs.h, which registers every module that got built. The core
# repo's own `zen` target is switched off at the top level (ZEN_BUILD_CLI).

set(ZEN_BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

add_executable(zen ${CMAKE_SOURCE_DIR}/vendor/zenvm/cli/main.cpp)
set_target_properties(zen PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ZEN_BIN_DIR})
target_include_directories(zen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(zen PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_definitions(zen PRIVATE
    $<$<CONFIG:Release>:NDEBUG>
    "ZEN_CLI_EXTRA_LIBS_HEADER=<zen_modules_libs.h>")
target_link_libraries(zen zen_static)

# Each of these module targets carries its ZEN_ENABLE_* define as a PUBLIC
# compile definition, so linking it is what turns the matching #ifdef on
# inside zen_modules_libs.h — nothing else to keep in sync. (canvas is not
# here on purpose: it exports no ZEN_ENABLE_CANVAS, it is a zen_gl-side
# module, and the core CLI never carried it either.)
foreach(mod stb_image glfw sdl2 dnn sqlite gif)
    if(TARGET zen_module_${mod})
        target_link_libraries(zen zen_module_${mod})
    endif()
endforeach()

if(NOT WIN32)
    target_link_options(zen PRIVATE -rdynamic)
else()
    # Windows defaults to a 1 MB stack; the compiler's nested-function
    # CompilerState frames need the 8 MB Linux gives by default.
    target_link_options(zen PRIVATE -Wl,--stack,8388608)
endif()
