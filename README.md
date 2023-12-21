# Cesources

Cesources is a small utility that can aid in embedding resource files into a [C][c-lang] project executable. When given a list of resource files, cesources will generate a single C source file with the contents of each resource file made available as unsigned char arrays that are named based on the resource file names. You can access these arrays by including the main cesources header file and using the given variable declaration macro to declare all of your expected resource variables.

[c-lang]: https://en.wikipedia.org/wiki/C_(programming_language)

### Development Setup

#### Manual Setup

To build this project, you'll need a C compiler and [CMake][cmake]. For example, if you want to use [GCC][gcc] on [Fedora Linux][fedora], you would run the following:

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

[dockerfile]: https://docs.docker.com/engine/reference/builder/
[oci]: https://opencontainers.org/
[toolbox]: https://containertoolbx.org/
