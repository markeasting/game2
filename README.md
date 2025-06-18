
# Installation
`conan install . --build=missing`

`conan install . --build=missing --settings=build_type=Debug` - debug deps

`mkdir build`

`cd build`

`cmake .. --preset conan-release`

`cd Release`

`make`

## Handy stuff
https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/cmake-settings.md#command-substitution 

