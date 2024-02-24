# SimpleGLTF

A usage example of [cGLTF](https://github.com/jkuhlmann/cgltf), which only loads a GLTF and displays it.

## Building

First install all dependencies needed to build GLFW3 and cGLTF from source, then fetch code with submodules:

```sh
git clone --recurse-submodules https://github.com/cedmundo/SimpleGLTF.git
```

Build with CMake:
```sh
mkdir build
cmake -B build -S .
```

Done.
