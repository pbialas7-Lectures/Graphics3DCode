<style>
.center {
  display: block;
  margin: 1em auto;
  width: 50%;
}
</style>

# House

#### 2 points

1. Please copy the `src/Assignments/00_Triangle`  directory to  `src/Assignments/01_House`. You can do this either by
   provided script (use it at your own risk!):
    ```{python}
    python3 ./scripts/copy_assignment.py 00_Triangle 01_House
    ```
   or "by hand." For this purpose

    1. In the folder  `src/Assignments`  copy folders using
       ```
       cp -r 00_Triangle 01_House
       ```
    2. Next switch to this newly created folder and in file  `CMakeLists.txt`  change the name of the project
       to  `House`.

   Please note that while the directory names should have numbers prefixed to them, the project name should not.

2. Rebuild everything by issuing command
    ```
    cmake .. && make -j 4
    ```
   in `./build` directory. Or just use VS Code or your other favorite IDE.


3. If everything goes all right, then after executing the program
    ```
    ./build/src/Assignments/01_House/House
    ```
   you should again see the red triangle.
   You are now ready to play with the code.
   Please note when using CLion executable is located in `./cmake-build-debug/src/Assignments/01_House/` directory.

4. Find the place where the background color is set.
   Change it to something else.
   Change it back to light-gray, for better debugging do not use black or white.

5. Find where in fragment shader the color of the triangle is set.
   Change it to something else.
   Change it back to red.

4. Find the place in `app.cpp` file where positions of vertices are stored. Change the location of the
   vertices. What happens when one of the coordinates _x,y_ is outside the range [-1,1]? What if  _z_ coordinate is
   outside this range?

6. Add one more triangle. Remember to edit draw command in the `frame` function.

7. Draw a house
   by adding beneath the original triangle a rectangle of width 1.0 and height 0.5 made out of two triangles. This
   should be your final version that you should submit to repository. It should look like this:

<img alt="House" src="house.png" class="center">