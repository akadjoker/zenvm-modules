/*
** zen_modules_libs.h — registers the native modules into the CLI VM.
**
** Included by vendor/zenvm/cli/main.cpp through ZEN_CLI_EXTRA_LIBS_HEADER
** (see cli/CMakeLists.txt). Each #ifdef is a ZEN_ENABLE_* define that the
** corresponding zen_module_* target exports PUBLIC when linked, so a module
** that wasn't built simply isn't registered — no manual list to maintain.
*/
#pragma once

#include "vm.h"
#include "module.h"
#ifdef ZEN_ENABLE_STB_IMAGE
#include "zen/module_image.h"
#endif
#ifdef ZEN_ENABLE_GLFW
#include "zen/module_glfw.h"
#endif
#ifdef ZEN_ENABLE_SDL2
#include "zen/module_sdl2.h"
#endif
#ifdef ZEN_ENABLE_NN
#include "zen/module_dnn.h"
#endif
#ifdef ZEN_ENABLE_SQLITE
#include "zen/module_sqlite.h"
#endif
/* gif is declared in module.h (no separate header). */

inline void zen_cli_register_extra_libs(zen::VM &vm)
{
    using namespace zen;
    (void)vm;
#ifdef ZEN_ENABLE_NN
    vm.register_lib(&zen_lib_nn);      /* modules/dnn */
#endif
#ifdef ZEN_ENABLE_SQLITE
    vm.register_lib(&zen_lib_sqlite);
#endif
#ifdef ZEN_ENABLE_GIF
    vm.register_lib(&zen_lib_gif);
#endif
#ifdef ZEN_ENABLE_STB_IMAGE
    vm.register_lib(&zen_lib_image);
#endif
#ifdef ZEN_ENABLE_GLFW
    vm.register_lib(&zen_lib_glfw);
#endif
#ifdef ZEN_ENABLE_SDL2
    vm.register_lib(&zen_lib_sdl2);
#endif
}
