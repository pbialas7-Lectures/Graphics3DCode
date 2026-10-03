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

This project uses CMake to set up and build the project.
As a part of the setup process, CMake will download a number of dependencies.
This can take some time, so be patient. The way to build the project is described below.

### "Plain vanilla" (Linux/Unix via command line)

The project is managed by CMake and can be built via the command line.
This should work for Linux/Unix.
I have not tested the command line build on Windows.

You will need a C++ compiler, CMake, Git and the development files for OpenGL, X11 and Wayland, which are needed
to compile the GLFW library. On Debian and derivatives like Ubuntu and Linux Mint you can install them with

```shell
sudo apt install build-essential cmake git pkg-config libgl-dev xorg-dev libwayland-dev libxkbcommon-dev
```

and on Fedora with

```shell
sudo dnf install gcc-c++ cmake git pkgconf mesa-libGL-devel wayland-devel libxkbcommon-devel libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel
```

Then change to the cloned repository and run:

```shell
mkdir build
cd build
cmake ..
make -j 
./src/Assignments/00_Triangle/Triangle
```

### VS Code

While you may work via command line and your preferred text editor, it is much more comfortable to use an IDE. I
recommend [Visual Studio Code](https://code.visualstudio.com/) which is available on Linux and Windows.

After installing VS Code, use it to open a folder containing the project repository.
You should install the recommended extension. The list is in the `.vscode/extensions.json` file, but you should be
prompted to do this after opening the
project folder. Also on opening, you may be prompted to configure the project.
You will have to choose the kit used for compilation, you will need a C++17 compiler. On Linux I am using
`clang` (10 or higher) but you can also use  `g++`.

On Windows if you do not have some version of Visual Studio installed, you will need to install
[Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) edition. 
If you have VS Community installed, then a suitable kit should appear in
the list of kits. After choosing it, the configuration and build should proceed without problems.

### CLion

While recommending VS Code, I personally use [CLion](https://www.jetbrains.com/clion/).
It is a commercial product, but you can get a free license if you are a student.
You can get the license [here](https://www.jetbrains.com/community/education/#students).
Setting up the CLion is similar to setting up VS Code.
Just open the project folder in CLion and it should configure itself.
You will have to choose the compiler kit. On Linux I am using clang (10 or higher) but you can also use g++.

## Running

Each assignment is built into a separate program in the `src/Assignments/<assignment>` folder of the build directory,
e.g. `build/src/Assignments/00_Triangle/Triangle`. In VS Code and CLion choose the program to run from the list of
targets.

The program opens a window with a small "Info" panel in the top left corner showing the number of frames per second.
The following keyboard shortcuts work in every program:

| Shortcut | Action                                                                                                    |
|----------|-----------------------------------------------------------------------------------------------------------|
| Ctrl-Q   | Close the window and end the program.                                                                     |
| Ctrl-S   | Save the next frame to `screenshot_<n>.png` in the current working directory, without the "Info" panel.   |
| Ctrl-F   | Capture the next frame in [RenderDoc](https://renderdoc.org/). Works only on Linux, when the program was started from RenderDoc. |

Messages, including OpenGL errors, are printed on the console. How to use them to find errors in your code is
described in [DEBUGGING.md](./Assignments/DEBUGGING.md).

## Assignments

The procedure for starting a new assignment is described in [README.md](./Assignments/README.md) file in
the `Assignments` folder.

