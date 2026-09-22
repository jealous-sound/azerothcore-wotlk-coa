# Build & tests

Out-of-source build is required (in-source is blocked).

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/azeroth-server -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DSCRIPTS=static -DMODULES=static
make -j$(nproc) && make install
```

C++20 required (`CMAKE_CXX_STANDARD 20`). Useful flags: `BUILD_TESTING=ON` (Google Test), `NOPCH=1` (disable precompiled headers). Full set in `conf/dist/config.cmake`. `compile_commands.json` is exported automatically.

Tests (Google Test, in `src/test/`): configure `-DBUILD_TESTING=ON`, then `ctest` or `./src/test/unit_tests` from the build dir.

## Source discovery and runtime checks

- Adding a module `.cpp` can require reconfiguring the existing CMake build to refresh source discovery.
  A successful incremental build does not prove the new file was compiled. Reconfigure when discovery is needed.
- CoA client DBCs belong in `env/dist/data/dbc`; point the worldserver `DataDir` at `env/dist/data` (Docker does).
  CoA tests in `apps/coa-tests/` read the same directory, or `COA_DBC_DIR` when set.
- CoA needs the Boost.PropertyTree headers (`boost-property-tree` for component-based vcpkg installs).
- Server readiness does not exercise character loading or gameplay. For lifecycle fixes, use a focused regression
  for the failing callback and relevant map states; report real login and in-game acceptance separately.
