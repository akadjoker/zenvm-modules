# zenvm-modules

Native modules, runners and graphics demos for the [Zen](https://github.com/akadjoker/zenvm)
language. The core VM — compiler, interpreter, builtin libraries and the
plain `zen` CLI — is the **zenvm** repository, consumed here as a git
submodule at `vendor/zenvm`. Everything in this repo links against its
`zen_static` target.

Extracted from `akadjoker/zenvm@541c023` (native modules, runners, graphics
demos) and `akadjoker/zenvm@ec7b4ee` (docs, examples/scripts, bench, tools,
the `plugins/hello` embedding demo — everything the core repo trimmed down
to just `libzen`/`cli`/`vendor`/`tests`). History before those commits
lives in zenvm.

## Layout

| Path | What |
|---|---|
| `modules/` | The native modules: `audio` (miniaudio), `canvas`, `dnn`, `gif`, `glfw`, `opengl`, `raylib`, `sdl2`, `sqlite`, `stb_image`, `stb_perlin`, `stb_rectpack`, `stb_trutype` |
| `cli/` | The batteries-included `zen` — same `main.cpp` as the core, with the modules registered (see below) |
| `runner/` | `zen_game`: zen + raylib, process loop |
| `runner_gl/` | `zen_gl`: zen + glfw + opengl + stb_* + audio + sdl2 + canvas_gl |
| `test_visual*/`, `test_gfx/`, `test_render/` | raylib-based benchmarks and the graphics-engine prototype |
| `examples/`, `scripts/` | Language demos (chip8, pascal, physics2d, tutorials) and scripts needing the graphics modules (`gl`, `flixel`) |
| `assets/` | Images and sounds used by the demos |
| `bench/` | Algorithm benchmarks, incl. vs. Lua/Wren |
| `tools/` | The Python editor prototype |
| `plugins/hello` | Minimal `zen_plugin.h` embedding demo — a loadable shared library (`dlopen`'d at runtime by `import hello;` in `tests/test_plugin.zen`); build it by hand, it isn't wired into the CMake build |
| `help/`, `std/` | CLI help text and `std/functional.zen` |
| `tests/` | Module snapshot tests (`run_zen_tests.sh bin/zen`) |
| `docs/` | Language guide, syntax reference, tutorials, and `modules.md` (script-facing API of each module) |
| `cmake/SystemRaylib.cmake` | Finds a system raylib for the raylib-based targets |

## Build

```sh
git clone --recursive git@github.com:akadjoker/zenvm-modules.git
cd zenvm-modules
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
bin/zen -e 'import sqlite; print("ok");'
```

If you cloned without `--recursive`: `git submodule update --init --recursive`.

Each module has a `ZEN_ENABLE_*` option (see the root `CMakeLists.txt`);
the graphics/audio ones need their system library (raylib, SDL2, GLFW,
OpenGL) and are skipped with a warning when it isn't found. The headless
subset (stb_*, dnn, sqlite, gif, canvas) builds anywhere.

## How the CLI picks up the modules

There is no second copy of the CLI. `cli/CMakeLists.txt` compiles
`vendor/zenvm/cli/main.cpp` with
`-DZEN_CLI_EXTRA_LIBS_HEADER=<zen_modules_libs.h>`; that header registers
every module whose target was built. Each `zen_module_*` target exports its
`ZEN_ENABLE_*` define publicly, so linking a module is what switches its
registration on — nothing to keep in sync by hand. The core repo's own `zen`
target is turned off here (`ZEN_BUILD_CLI=OFF`) to avoid the name clash.

## Updating the core

```sh
cd vendor/zenvm && git fetch && git checkout <commit-or-tag> && cd ../..
git add vendor/zenvm && git commit -m "bump zenvm to <commit>"
```
