# Colors
Termin oddania: 27.10.21 23:59 5 pkt. 

1. Start the assignment by copying the `House` assignment folder to `Colors` folder as described  previously. 
2. Now add the color data to the vertices. In the array holding the vertices, after coordinates of each vertex add a RGB color value. Please remember that in OpenGL the colors channels  take values from the interval [0,1]. 
   So the data in vertex buffer should be arranged like this:    
   ```
   x,y,z,r,g,b,x,y,z,r,g,b,x,y,z,r,g,b,...
   ```
3. After compiling and  running the application you will probably see something very strange or nothing at all. This is because we have changed the format of the data in the vertex buffer. Please modify the `glVertexAttribPointer` function call to make it work again (this requires changing a single parameter).

4. If the house apears on the screen we can start connecting the colors from the vertex buffer. To this end please add an input variable (attribute) for colors in the vertex shader code with `location=1`.  Please verify if everythin is still working. 

5. In the cpu code add  apropriate `glVertexAttribPointer` and `glEnableVertexAttribArray` that will enable for colors to be read from the vertex buffer. Everyting should still be working fine. 

6. Finally we have to pass colors from vertex shader to fragment shader. To this end add an output variable for color in the vertex shader and in the `main` function assign color attribute to it. Then add an input variable in fragment shader with **same type and name**. And finally in main function assign this variable to the  output of the fragment shader. 

7. Play with colors to see how the colors are interpolated.

8. Color the roof of the house red and the walls green. 
   

