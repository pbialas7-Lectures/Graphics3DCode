# A 3D graphics programming project.

This repository contains the "Hello World!" equivalent for OpenGL C++ programming. This will be the starting point for
your assignments.

## Downloading

To download the project you have to clone the repository

```shell
git clone https://github.com/pbialas7-Lectures/Graphics3DCode.git
```

## OpenGL version

In this project I am using OpenGL 4.6. You will need a graphics card/driver that supports this version. You can check
the version of OpenGL supported by your graphics card/driver using
the [OpenGL Extensions Viewer](https://www.realtech-vr.com/glview/). If for some reason you cannot use OpenGL 4.6 (see
Apple below) you can change the version in the `CMakeLists.txt` file by setting the different value for `MINOR`
variable. Using version 4.5 should be fine.

### Apple

Unfortunately, Apple does not support OpenGL 4.6. The highest version supported by Apple is 4.1.
CMake will detect this and set the version to 4.1.
This will cause problems with the assignments. The possible solutions will be given were needed.

## Building

### "Plain vanilla" (Linux/Unix via command line)

The project is managed by CMake and can be build via the commandline. This should work for Linux/Unix. I have not tested
the command line build on Windows.

Use CMake to compile. First change to the cloned repository and then run:

```shell
mkdir build
cd build
cmake ..
make -j 
./src/Assignments/00_Triangle/Triangle
```

### VS Code

While you may work via command line and your preferred text editor it is much more comfortable to use an IDE. I
recommend [Visual Studio Code](https://code.visualstudio.com/) which is available on Linux, macOS and Windows.

After installing VS Code, use it to open a folder containing the project repository.
You should install the recommended extension. The list is in the `.vscode/extensions.json` file, but you should be
prompted to do this after opening the
project folder. Also on opening, you may be prompted to configure the project.
You will have to choose the kit used for compilation, you will need a C++17 compiler. On Linux and macOS I am using
clang (10 or higher) but you can also use
g++.

On Windows if you do not have some version of Visual Studio installed, you will need to install
either [Visual Studio Community](https://visualstudio.microsoft.com/pl/vs/community/) edition
or [Mingw-gw64](https://www.mingw-w64.org/). If you have VS Community installed, then a suitable kit should appear in
the list of kits. After choosing it, the configuration and build should proceed without problems.

### Using Mingw-gw with Visual Studio Code

To install Mingw-gw64 follow the instructions
on [VS Code Documentation](https://code.visualstudio.com/docs/cpp/config-mingw). Follow steps 1-4 from the
prerequisites.

After that, you should have a new kit visible in VS Code. However, if you try to configure and build a project, you will
get an error that CMake generator cannot be found. That is because the `make.exe` command in the Mingw-gw64 distribution
is
called `mingw32-make.exe`. To fix this, you will need to make a symbolic link. Open the command prompt (In start menu
type `cmd`). Then navigate to `C:\msys64\mingw64\bin` folder and type:

```shell
mklink make.exe `mingw32-make.exe
```

### CLion

While recommending VS code, I personally use [CLion](https://www.jetbrains.com/clion/).
It is a commercial product, but you can get a free license if you are a student.
You can get the license [here](https://www.jetbrains.com/community/education/#students).
Setting up the CLion is similar to setting up VS Code.
Just open the project folder in CLion and it should configure itself.
You will have to choose the compiler kit. On Linux and macOS I am using clang (10 or higher) but you can also use g++.

## Assignments

The procedure for starting a new assignment is described in [README.md](./Assignments/README.md) file in
the `Assignments` folder.

