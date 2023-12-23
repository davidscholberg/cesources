# Cesources

Cesources is a small utility that can aid in embedding resource files into a [C][c-lang] project executable. When given a list of resource files, cesources will generate a single C source file with the contents of each resource file made available as unsigned char arrays that are named based on the resource file names. You can access these arrays by including the main cesources header file and using the given variable declaration macro to declare all of your expected resource variables.

Both cesources itself and the C source files it generates aim to be [C99][c99] compliant. So far cesources has only been verified to work on [GNU/Linux][gnu-linux] with [GCC][gcc], but in its current form it should in theory work on other platforms and compilers (although note that the [CMake][cmake] config currently has a few GCC-isms hardcoded into it).

[c-lang]: https://en.wikipedia.org/wiki/C_(programming_language)
[c99]: https://en.wikipedia.org/wiki/C99
[gnu-linux]: https://www.gnu.org/gnu/linux-and-gnu.en.html

### Development Setup

#### Manual Setup

To build this project, you'll need a C99-compliant C compiler and optionally [CMake][cmake]. For example, if you want to use [GCC][gcc] and CMake on [Fedora Linux][fedora], you would run the following:

```bash
sudo dnf install gcc cmake
```

[cmake]: https://cmake.org/
[gcc]: https://gcc.gnu.org/
[fedora]: https://fedoraproject.org/

#### Fedora Toolbox Setup

This project contains a [Dockerfile][dockerfile] that allows you to easily build an [OCI][oci] image with all of the build dependencies of cesources included. The resulting image is intended to be used with [toolbox][toolbox]. The following commands will build and run the image:

```bash
podman build --tag cesources --file Dockerfiles/Dockerfile.fedora-toolbox
toolbox create --image cesources cesources
toolbox enter cesources
```

### Building Cesources

Once you have installed the build dependencies from the [Development Setup][#development-setup] section, you can either build the project via CMake or build it manually.

#### Build Via CMake

If your development environment has CMake support, then you should refer to the relevant documentation for building CMake projects within it. Otherwise, you can build the project with CMake on the command line like so:

```bash
cd /path/to/cesources
mkdir -p build && cd build && cmake .. && make
```

#### Build Manually

If you'd like to build the project manually, this is an example command for GCC:

```bash
cd /path/to/cesources
mkdir -p build && gcc -O3 -DNDEBUG -std=c99 -Wall -Wextra -Werror -Wpedantic -o build/cesources src/*.c
```

[dockerfile]: https://docs.docker.com/engine/reference/builder/
[oci]: https://opencontainers.org/
[toolbox]: https://containertoolbx.org/
