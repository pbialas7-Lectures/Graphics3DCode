# Guidelines

1. Do your work in small incremental steps. Build and run the program after each step, so that you know which change
   broke it.
2. Wrap all calls to the OpenGL API with the `OGL_CALL` macro. It checks for an OpenGL error after the call and stops
   the program at the first error, showing the call, the file and the line where it happened (see
   [DEBUGGING.md](DEBUGGING.md)). The macro is a single statement, so a variable declared inside it is not visible
   after it. If you need the value returned by a function, declare the variable first and assign it inside the macro:

   ```c++
   GLint location;
   OGL_CALL(location = glGetUniformLocation(program, "map_Kd"));
   ```

3. Commit your changes often. This will allow you to go back to a working version if you break something.
   Push them to your GitHub repository regularly, as this is where your assignments are checked.
4. Do not leave commented out code in your programs.
