# A 3D graphics programming project.

This repository contains the "Hello World!" equivalent for OpenGL C++ programming. This will be the starting point for
your assignments.

## Downloading

To download the project you have to clone the repository

```shell
git clone https://github.com/pbialas7-Lectures/Graphics3DCode.git
```

## OpenGL version

In this project, I will be using OpenGL 4.6.
The minimum version required is 4.5.
You will need a graphics card/driver that supports this version.
You can check
the version of OpenGL supported by your graphics card/driver using
the [OpenGL Extensions Viewer](https://www.realtech-vr.com/glview/). If for some reason you cannot use OpenGL 4.6 (see
Apple below) you can change the version in the `CMakeLists.txt` file by setting the different value for `MINOR`
variable. Using version 4.5 should be fine.

### Apple

Unfortunately, Apple does not support OpenGL 4.6.
The highest version supported by Apple is 4.1.
So I am sorry to say it will be impossible to use Apple computers with macOS for
this course :(

## Building

This project uses CMake to set up and build the project.
As a part of the setup process, CMake will download a number of dependencies.
This can take some time, so be patient. The way to build the project is described below.

### "Plain vanilla" (Linux/Unix via command line)

The project is managed by CMake and can be built via the commandline.
This should work for Linux/Unix.
I have not tested the command line build on Windows.

First change to the cloned repository and then run:

```shell
mkdir build
cd build
cmake ..
make -j 
./src/Assignments/00_Triangle/Triangle
```

### VS Code

While you may work via command line and your preferred text editor, it is much more comfortable to use an IDE. I
recommend [Visual Studio Code](https://code.visualstudio.com/) which is available on Linux, macOS and Windows.

After installing VS Code, use it to open a folder containing the project repository.
You should install the recommended extension. The list is in the `.vscode/extensions.json` file, but you should be
prompted to do this after opening the
project folder. Also on opening, you may be prompted to configure the project.
You will have to choose the kit used for compilation, you will need a C++17 compiler. On Linux and macOS I am using
`clang` (10 or higher) but you can also use  `g++`.

On Windows if you do not have some version of Visual Studio installed, you will need to install
either [Visual Studio Community](https://visualstudio.microsoft.com/pl/vs/community/) edition. 
If you have VS Community installed, then a suitable kit should appear in
the list of kits. After choosing it, the configuration and build should proceed without problems.

### Using Mingw-gw with Visual Studio Code on Windows

It should be also possible to use the [Mingw-gw64](https://www.mingw-w64.org/) compiler.
However, I have not tested this setup this year.
I will let you know as soon as I manage to do this.

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

