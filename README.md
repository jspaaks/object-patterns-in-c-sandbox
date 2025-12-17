[![Copier](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/copier-org/copier/master/img/badge/badge-grayscale-inverted-border-orange.json)](https://github.com/copier-org/copier)

# mbm

## CMake

The project has been initialized with a [CMakeLists.txt](CMakeLists.txt)-based
configuration for building with CMake:

```console
# change into the build directory
$ cd build/

# generate the build files
$ cmake -DCMAKE_BUILD_TYPE=Debug ..

# build the project
$ cmake --build .

# install the project to <repo>/build/dist
$ cmake --install . --prefix dist/

# run the program to see if it works
$ ./dist/bin/mbm
```

Should output something like:

```text
$ mbm
```

## Testing

Building and running the tests requires that [Criterion](https://github.com/Snaipe/Criterion) is
installed on the system, e.g. with

```console
$ sudo apt install libcriterion3 libcriterion-dev
```

Run the tests with

```console
$ ./dist/bin/test_mbm -j1 --verbose
```

The CMake variable `MBM_BUILD_TESTING` can be used to build the
tests.

- When this project is the top project, `MBM_BUILD_TESTING` inherits the value of
  CTest's `BUILD_TESTING`, which is set to ON by default.
- When this project is not the top project but instead it is used as a dependency to a parent
  project, the default is to not build the tests. However, building the tests is still possible by
  setting the `MBM_BUILD_TESTING` to `ON`, e.g like so:

```console
$ cmake -DMBM_BUILD_TESTING=ON ..
```

## Address sanitizing

To use address sanitizing, you may need to install an extra dependency, e.g. like so:

```console
# (Ubuntu)
sudo apt install libasan8
```

The CMake variable `MBM_APP_WITH_ASAN` can be used to enable address sanitizing on the
executable `mbm`. `MBM_APP_WITH_ASAN`'s value is `OFF` by default. To
enable it, configure the build via `ccmake ..`, or via a command line argument with:

```console
$ cmake -DAPP_WITH_ASAN=ON ..
```

The CMake variable `MBM_LIB_WITH_ASAN` can be used to enable address sanitizing on the
library `mbm`. `MBM_LIB_WITH_ASAN`'s value is `OFF` by default. To
enable it, configure the build via `ccmake ..`, or via a command line argument with:

```console
$ cmake -DMBM_WITH_ASAN=ON ..
```

## Acknowledgements

_This project was initialized using [Copier](https://pypi.org/project/copier) and the [copier-template-for-c-projects](https://github.com/jspaaks/copier-template-for-c-projects)._

<!--

Project limits for the time being

1. collision detection
1. paused gamestate
1. keybindings

TODO
- look into SDL_TEXTUREACCESS_STREAMING, SDL_UpdateTexture, SDL_LockTextureToSurface for efficient
  display of fpscounter
- look into gamestate-specific event handling
- look into offscreen rendering for tests
- look into https://wiki.libsdl.org/SDL3/SDL_GetRectIntersection for collisions
-->
