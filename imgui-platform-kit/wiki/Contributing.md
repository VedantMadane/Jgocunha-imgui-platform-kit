# Contributing

Contributions are welcome — bug reports, documentation improvements and pull requests.

## Reporting issues

Open an issue on the [GitHub issue tracker](https://github.com/Jgocunha/imgui-platform-kit/issues). Issues are automatically triaged and labelled by a CI bot; a clear title and a minimal reproduction help a lot.

## Pull request workflow

1. Fork the repository and create a descriptive branch (e.g. `feature/my-feature` or `fix/my-bug`).
2. Build and test locally — see [Building from Source](Building-from-Source). Tests are built by default (`IMGUI_PLATFORM_KIT_BUILD_TESTS=ON`) and run with `ctest --output-on-failure`.
3. Open a pull request against `main`.

## CI checks

Every pull request runs through GitHub Actions:

| Workflow | What it checks |
|---|---|
| Windows / Linux / macOS Build | The library, example and tests build on all three platforms |
| Coverage | Test coverage (uploaded to Codecov) |
| Documentation Sync Check | New or changed public APIs have Doxygen comments and the wiki stays in sync |

## Documentation requirements

All public symbols in `imgui-platform-kit/include/` carry Doxygen comments (`@brief`, `@param`, `@return`, …). If your change adds or modifies a public API:

- add or update the Doxygen comment block in the header;
- update the relevant wiki page if the narrative documentation is affected.

The API reference is generated with Doxygen (`doxygen Doxyfile` from the `imgui-platform-kit/` directory) and published to [GitHub Pages](https://jgocunha.github.io/imgui-platform-kit/).

## Adding a new platform backend

Platform backends live in `include/<platform>/` and `src/<platform>/` (see the existing `win32`, `linux` and `macos` backends). A backend must:

1. define `imgui_kit::UserInterface` with the same public API as the existing backends (`initialize()`, `render()`, `shutdown()`, `isShutdownRequested()`, `addWindow<T>()`), guarded by the appropriate platform `#if`;
2. be dispatched from `include/user_interface.h`;
3. get its own CI build workflow in `.github/workflows/`.
