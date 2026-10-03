# A 3D graphics programming project

This repository contains the "Hello World!" equivalent for OpenGL C++ programming. This will be the starting point for
your assignments.

## Downloading

To download the project you have to clone the repository with Git (how to install it is described
in [Building](#building) below):

```shell
git clone https://github.com/pbialas7-Lectures/Graphics3DCode.git
```

## OpenGL version

In this project, I will be using OpenGL 4.6.
The minimum version required is 4.5.
You will need a graphics card/driver that supports this version.
You can check the version of OpenGL supported by your graphics card/driver using
the [OpenGL Extensions Viewer](https://www.realtech-vr.com/glview/). On Linux you can also run `glxinfo -B`
(from the `mesa-utils` package) and look at the `OpenGL core profile version string` line.

If your graphics card/driver does not support OpenGL 4.6, change the line `set(MINOR 6)` in the top `CMakeLists.txt`
file to

```cmake
set(MINOR 5)
```

and configure the project again. Version 4.5 is fine for all the assignments.

### Apple computers are not supported

This project does not work on Apple computers with macOS. Apple supports OpenGL only up to version 4.1, below the
required minimum of 4.5, and it cannot be changed by setting `MINOR`. Please use a computer with Linux or Windows.

## Building

This project uses CMake (version 3.16 or newer) to set up and build the project.
As a part of the setup process, CMake will download a number of dependencies.
This can take some time, so be patient. You need an internet connection for this first configuration; the
dependencies are downloaded only once for each build folder.

First install the tools and libraries needed on your system, as described in the [Linux](#linux) or
[Windows](#windows) section below. Then build the project from the command line, in VS Code or in CLion.

### Linux

You will need a C++ compiler, CMake, Git and the development files for OpenGL, X11 and Wayland, which are needed
to compile the GLFW library. On Debian and derivatives like Ubuntu and Linux Mint you can install them with

```shell
sudo apt install build-essential cmake git pkg-config libgl-dev xorg-dev libwayland-dev libxkbcommon-dev
```

and on Fedora with

```shell
sudo dnf install gcc-c++ cmake git pkgconf mesa-libGL-devel wayland-devel libxkbcommon-devel libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel
```

### Windows

On Windows install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) and select the
**Desktop development with C++** workload in the installer; this provides the C++ compiler and CMake. You will also
need [Git for Windows](https://git-scm.com/downloads), because CMake uses Git to download the dependencies. Keep the
installer option that adds Git to the `PATH`.

### "Plain vanilla" (Linux via command line)

The project is managed by CMake and can be built via the command line.
This should work for Linux/Unix.
I have not tested the command line build on Windows.

After installing the packages listed in the [Linux](#linux) section, change to the cloned repository and run:

```shell
mkdir build
cd build
cmake ..
cmake --build . -j 4
./src/Assignments/00_Triangle/Triangle
```

The `-j 4` option builds with four parallel jobs; you can use more if your computer has enough cores and memory.

### VS Code

While you may work via command line and your preferred text editor, it is much more comfortable to use an IDE. I
recommend [Visual Studio Code](https://code.visualstudio.com/) which is available on Linux and Windows.

After installing VS Code, use it to open a folder containing the project repository.
You should install the recommended extensions. The list is in the `.vscode/extensions.json` file, but you should be
prompted to do this after opening the
project folder. Also on opening, you may be prompted to configure the project.
You will have to choose the kit used for compilation, you will need a C++17 compiler.
During the first configuration VS Code may also ask whether CMake Tools may configure IntelliSense (code completion
and error highlighting). Choose **Allow**, otherwise the editor will mark the includes of the downloaded libraries
as errors, although the project builds fine.

On Linux, install the packages listed in the [Linux](#linux) section first. I am using `clang` (10 or higher) but you
can also use `g++`.

On Windows, after installing Visual Studio Community with the C++ workload and Git (see [Windows](#windows) above),
a Visual Studio kit should appear in the list of kits. Choose the one ending in `amd64`, e.g.
*Visual Studio Community 2022 Release - amd64* (the year depends on your Visual Studio version), not the `x86` or
`arm64` ones. After choosing it, the configuration and build should proceed without problems.

To build and run an assignment, select it as the build and launch target in the CMake panel (the CMake icon in
the left bar) or in the status bar at the bottom of the window, e.g. `Triangle`. Then use the Build button to
compile it, the ▷ (Run) button to run it, and the bug icon to run it in the debugger, where you can set breakpoints
by clicking to the left of the line numbers. The same commands are available in the Command Palette (`Ctrl+Shift+P`)
as **CMake: Build**, **CMake: Run Without Debugging** and **CMake: Debug**.

### CLion

While recommending VS Code, I personally use [CLion](https://www.jetbrains.com/clion/).
It is a commercial product, but you can get a free license if you are a student.
You can get the license [here](https://www.jetbrains.com/community/education/#students).
Setting up CLion is similar to setting up VS Code.
Just open the project folder in CLion and it should configure itself.

On Linux, install the packages listed in the [Linux](#linux) section first. CLion will use the system compiler; I am
using clang (10 or higher) but you can also use g++.

On Windows, install Visual Studio Community with the C++ workload and Git (see [Windows](#windows) above). By default
CLion uses its own bundled MinGW compiler, so go to *Settings | Build, Execution, Deployment | Toolchains*, add a
*Visual Studio* toolchain and move it to the top of the list to make it the default.

## Running

Each assignment is built into a separate program in the `src/Assignments/<assignment>` folder of the build directory,
e.g. `build/src/Assignments/00_Triangle/Triangle`. With the Visual Studio compiler on Windows the program is in a
subfolder named after the build configuration, e.g. `build\src\Assignments\00_Triangle\Debug\Triangle.exe`.
In VS Code and CLion choose the program to run from the list of targets (see [VS Code](#vs-code) above).
When started from VS Code or CLion, the program usually runs in this build folder of the assignment, so files it
writes, such as screenshots, end up there, e.g. in `build/src/Assignments/00_Triangle/`.

The program opens a window with a small "Info" panel in the top left corner showing the number of frames per second.
It is usually equal to the refresh rate of your monitor, e.g. 60, because the program waits for the monitor before
displaying each frame; this is explained in [DOUBLE_BUFFERING.md](./DOUBLE_BUFFERING.md).
The following keyboard shortcuts work in every program:

| Shortcut | Action                                                                                                    |
|----------|-----------------------------------------------------------------------------------------------------------|
| Ctrl-Q   | Close the window and end the program.                                                                     |
| Ctrl-S   | Save the next frame to `screenshot_<n>.png` in the current working directory (see above), without the "Info" panel. |
| Ctrl-F   | Capture the next frame in [RenderDoc](https://renderdoc.org/), when RenderDoc's own capture key (F12) does not work. Linux only, see [DEBUGGING.md](./Assignments/DEBUGGING.md#renderdoc). |

Messages, including OpenGL errors, are printed on the console. How to use them to find errors in your code is
described in [DEBUGGING.md](./Assignments/DEBUGGING.md).

## Assignments

The procedure for starting a new assignment is described in the [README.md](./Assignments/README.md) file in
the `Assignments` folder.

