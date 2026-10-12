# Final Practice

OpenGL 3.3 project for Visual Studio 2022. The solution uses a checked-in
`vcpkg.json` manifest and a project-local bootstrap so a clean checkout can
restore every compiled dependency instead of relying on `.lib` files copied
manually from another computer.

## Requirements

- Visual Studio 2022 with **Desktop development with C++**.
- Git for Windows, either installed directly or provided by GitHub Desktop.
- An internet connection the first time dependencies are restored.

## Build

1. Open `project/visual-studio-2022/Final Practice.sln`.
2. Select `Debug | x64` or `Release | x64`.
3. Build the solution. MSBuild clones the pinned vcpkg revision into the
   ignored `.tools` directory and restores the manifest dependencies into the
   ignored `vcpkg_installed` directory automatically.
4. Run the project normally from Visual Studio.

The first build takes longer because vcpkg, SDL2, GLAD, GLM, SOIL2, Assimp and
their transitive dependencies are downloaded and compiled. Later builds reuse
the local package and binary caches. PowerShell is invoked with a project-local
script; no user-wide vcpkg integration is required.

The supported target is Windows x64. The Release executable distributed with
the project is stored in `binaries/executable/Final Practice.exe`.
