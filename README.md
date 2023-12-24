# Cesources

Cesources is a small utility that can aid in embedding resource files into a [C][c-lang] project executable. When given a list of resource files, cesources will generate a single C source file with the contents of each resource file made available as unsigned char arrays that are named based on the resource file names. You can access these arrays by including the main cesources header file and using the given variable declaration macro to declare all of your expected resource variables.

Both cesources itself and the C source files it generates aim to be [C99][c99] compliant. So far cesources has only been verified to work on [GNU/Linux][gnu-linux] with [GCC][gcc], but in its current form it should in theory work on other platforms and compilers (although note that the [CMake][cmake] config currently has a few GCC-isms hardcoded into it).

[c-lang]: https://en.wikipedia.org/wiki/C_(programming_language)
[c99]: https://en.wikipedia.org/wiki/C99
[gnu-linux]: https://www.gnu.org/gnu/linux-and-gnu.en.html

### Usage

You can use cesources manually as an external utility or you can integrate it into CMake.

#### Manually Build Embedded Resources

Build cesources as specified in the [Building Cesources](#building-cesources) section, and then run the cesources command to generate a single C file with all of the specified resources embedded into it:

```bash
cesources --input-dir /home/you/foo/resource_files/ --output-dir /home/you/foo/resource_files/ resource_file_1.txt resource_file_1.txt
```

Build the generated C file into an object file:

```bash
gcc -O3 -DNDEBUG -std=c99 -Wall -Wextra -Werror -Wpedantic -o /home/you/foo/resource_files/resources.c.o -c /home/you/foo/resource_files/resources.c
```

#### Build Embedded Resources With CMake

You can incorporate cesources into your CMake build with the following config:

```cmake
include(FetchContent)

FetchContent_Declare(
    cesources
    GIT_REPOSITORY https://github.com/davidscholberg/cesources.git
    GIT_TAG 3b6bb46f172c411a61c95890b8b2acc91a0f7bfb
)

FetchContent_MakeAvailable(cesources)

# Create an explicit dependency for your project on cesources
add_dependencies(
    foo
    cesources
)

# Allow cesources public header to be included in your project
target_include_directories(
    foo
    PRIVATE
    ${cesources_SOURCE_DIR}/src/include
)

# Create list of all resource files to embed
set(
    resource_filenames
    resource_file_1.txt
    resource_file_2.txt
)

# Add command to build embedded resource file and make it depend on cesources itself and the
# resource files
add_custom_command(
    OUTPUT resources.c
    COMMAND cesources --input-dir ${CMAKE_CURRENT_SOURCE_DIR}/ ${resource_filenames}
    DEPENDS cesources ${resource_filenames}
)

# Build the generated resource file as an object library
add_library(
    foo_resources
    OBJECT
    resources.c
)

set_target_properties(
    foo_resources
    PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_compile_options(
    foo_resources
    PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wpedantic
)

# Add the resources object file to your executable
target_sources(
    foo
    PRIVATE
    $<TARGET_OBJECTS:foo_resources>
)
```

#### Access Embedded Resources In Code

Below is some example code for accessing the resources embedded by cesources:

```c
#include <cesources.h>

// Use the variable declaration macro to declare the expected resource variables. For each resource,
// the macro will declare a const unsigned char array and an integer holding the array size. Both of
// the variables will be declared as extern, since they are defined in the embedded resource file.
cesources_declare_var(resource_file_1_txt);

void foo(void) {
    bar(resource_file_1_txt, resource_file_1_txt_size);
}
```

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

Once you have installed the build dependencies from the [Development Setup](#development-setup) section, you can either build the project via CMake or build it manually.

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
