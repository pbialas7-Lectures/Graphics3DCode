# A 3D graphics programming project.

This repository contains the "Hello World!" equivalent for OpenGL C++ programming.  This will be the starting point for your assignments. 

## Downloading 

To download the project you have to clone the repository

```shell
git clone https://github.com/pbialas7-Lectures/Graphics3DCode.git
```

## Building 

### "Plain vanilla" (Linux/Unix via command line)

The project is managed by CMake and can be build cia tyhe commandline. This should work for Linux/Unix. I have not tested the command line build on Windows.  

Use CMake to compile. First change to the cloned repository and then run:

```shell
mkdir build
cd build
cmake ..
make -j 
./src/Assignments/Triangle
```

### VS Code

While you may work via command line and your prefered text editor it is much more comfortable to use an IDE. I recommend [Visual Studio Code](https://code.visualstudio.com/) which is available on Linux, Mac OS and Windows.   

After installing VS Code use it to open folder containing the  project repository. You should install the  recommended extension. The list is in the `.vscode/extensions.json` file, but you should be prompted to do this after opening the project folder. Also on opening you may by prompted to configure the project. You will have to choose the the kit used for compilation, you will need a C++17 compiler. On Linux and macOS I am using clang (10 or higher) but you can also use g++. 

On Windows you will need to install either [Visual Studio Community](https://visualstudio.microsoft.com/pl/vs/community/) edition or [Mingw-gw64](https://www.mingw-w64.org/). If you have VS Community installed then suitable kit should apear in the list of kits. After choosing it the configuration and build should proceed without problems.

### Using Mingw-gw with Visual Studio Code

To instal Mingw-gw64 follow the  instructions on [VS Code Documentation](https://code.visualstudio.com/docs/cpp/config-mingw). Follow steps 1-4 from the prerequisites. 

After that  you should have a new kit visible in VS Code. However if you try to configure and build project you will get an error that CMake generator cannot be found. That is because the `make.exe` command in the Mingw-gw64 distribution is called `mingw32-make.exe`. To fix tghis you will need to make a symbolink link. Open the command prompt (In start menu type `cmd`). The navigate to `C:\msys64\mingw64\bin` folder and type:

```shell
mklink make.exe `mingw32-make.exe
```
## Assignments

Before starting each assignment you should copy the directory containing previous assignment. Specyfically you should not modify anything in `src/Assignments/Triangle` folder, but copy it to `src/Assignments/House`  folder. You can do it using `cp` command (on linux/unix). 

```shell
cp -r scr/Assignments/Triangle  scr/Assignments/House
```

Then you should modify the project name in `src/Assignments/House/CMakeLists.txt` from `Triagle` to `House`.

I have provided a python script  that you can use for this:

```shell
python ./scripts/copy_assignment.py Triangle House
```

Use at your own risk!

### Setting your repository

You should create you own private repository on github. Then change the name of the remote repository in Graphis3Code

```shell
git remote rename origin origin-lecture
```

and add your repository as remote

```shell
git remote add origin url-to-your-repository
```

and finally push everything to  your new repository

```shell
git push -u origin main
```

Invite me (pbialas7) to your repository and then
add the url of the repository (the ssh not htpps) to the repositories spredsheat on Teams.

That way I can access you files for grading and helping you debug. 

