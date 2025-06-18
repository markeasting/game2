`conan install . --build=missing`

`conan install . --build=missing --settings=build_type=Debug` - debug deps

`mkdir build`

`cd build`

`cmake .. --preset conan-release`

`cd Release`

`make`
